<?php

namespace iggyvolz\discordsocial;

use Amp\Cancellation;
use Amp\DeferredFuture;
use Amp\Future;
use Closure;
use Psr\EventDispatcher\EventDispatcherInterface;
use Psr\Log\LoggerInterface;
use Psr\Log\LogLevel;
use Psr\Log\NullLogger;

final class Client {
    /** @var list<string> */
    public array $DefaultPresenceScopes {get => self::getDefaultPresenceScopes();}

    /** @return list<string> */
    public static function getDefaultPresenceScopes(): array {}
    /** @var list<string> */
    public array $DefaultCommunicationScopes {get => self::getDefaultCommunicationScopes();}

    /** @return list<string> */
    public static function getDefaultCommunicationScopes(): array {}

    public function __construct(
        private readonly LoggerInterface $logger = new NullLogger(),
        private readonly EventDispatcherInterface $eventDispatcher = new NullEventDispatcher(),
    ) {
        $this->addLogCallback(function (string $message, LoggingSeverity $severity) {
            $this->logger->log(
                match ($severity) {
                    LoggingSeverity::Verbose => LogLevel::DEBUG,
                    LoggingSeverity::Info => LogLevel::INFO,
                    LoggingSeverity::Warning => LogLevel::WARNING,
                    LoggingSeverity::Error => LogLevel::ERROR,
                    LoggingSeverity::None => throw new \LogicException(),
                },
                $message,
            );
        }, LoggingSeverity::Verbose);
        $this->statusChangedCallback = fn (Status $status, Error $error, int $code) =>
            $this->eventDispatcher->dispatch(new StatusChangedEvent($status, $error, $code));
    }
    public function __destruct() {}
    public static function runCallbacks(): void {}
    /** @param Closure(string, LoggingSeverity):void $callback */
    private function addLogCallback(Closure $callback, LoggingSeverity $loggingSeverity): void {}

    /** @var Closure(Status, Error, int):void $callback */
    private Closure $statusChangedCallback {set {}}
    /** @return Future<AuthorizeResult> */
    public function authorize(int $clientId, array $scopes, AuthorizationCodeChallenge $challenge, ?string $nonce = null, ?string $state = null, ?Cancellation $cancellation = null): Future {
        $futureState = new DeferredFuture();
        $cancellation?->subscribe(fn() => $this->abortAuthorize());
        $this->_authorize($clientId, $scopes, $challenge, function(
            ClientResult $result,
            string $code,
            string $redirectUri
        ) use($futureState){
            if(!$result->successful) {
                // TODO improve this - code 5000 is a denial & should get its own case
                throw new \RuntimeException("Auth Error: $result->error");
            }
            $futureState->complete(new AuthorizeResult($code, $redirectUri));
        }, $nonce, $state);
        return $futureState->getFuture();
    }
    private function _authorize(int $clientId, array $scopes, AuthorizationCodeChallenge $challenge, Closure $callback, ?string $nonce, ?string $state, ?Cancellation $cancellation = null): void {}
    private function abortAuthorize(): void {}
    public function createAuthorizationCodeVerifier(): AuthorizationCodeVerifier {}
    /** @return Future<GetTokenResult> */
    public function getToken(int $clientId, string $code, string $verifier, string $redirectUri): Future {
        $futureState = new DeferredFuture();
        $this->_getToken($clientId, $code, $verifier, $redirectUri, function(
            ClientResult $result,
            string $accessToken,
            string $refreshToken,
            AuthorizationTokenType $tokenType,
            int $expiresIn,
            string $scopes,
        ) use($futureState){
            if(!$result->successful) {
                throw new \RuntimeException("Get Token Error: $result->error");
            }
            $futureState->complete(new GetTokenResult($accessToken, $refreshToken, $tokenType, $expiresIn, explode(" ", $scopes)));
        });
        return $futureState->getFuture();
    }

    private function _getToken(
        int $clientId,
        string $code,
        string $verifier,
        string $redirectUri,
        Closure $callback
    ) {}

    /** @return Future<void> */
    public function updateToken(AuthorizationTokenType $tokenType, string $accessToken): Future {
        $futureState = new DeferredFuture();
        $this->_updateToken($tokenType, $accessToken, function(
            ClientResult $result,
        ) use($futureState){
            if(!$result->successful) {
                throw new \RuntimeException("Get Token Error: $result->error");
            }
            $futureState->complete();
        });
        return $futureState->getFuture();
    }

    private function _updateToken(AuthorizationTokenType $tokenType, string $accessToken, Closure $param) {}

    public function connect(): void {}
    /** @return Future<void> */
    public function updateRichPresence(Activity $activity): Future {
        $futureState = new DeferredFuture();
        $this->_updateRichPresence($activity, function(
            ClientResult $result,
        ) use($futureState){
            if(!$result->successful) {
                throw new \RuntimeException("Update Rich Presence Error: $result->error");
            }
            $futureState->complete();
        });
        return $futureState->getFuture();
    }

    private function _updateRichPresence(Activity $activity, Closure $callback) {}
    public int $applicationId {get{} set{}}
}