<?php

use CuyZ\Valinor\Mapper\Source\JsonSource;
use CuyZ\Valinor\MapperBuilder;
use iggyvolz\discordsocial\Activity;
use iggyvolz\discordsocial\AuthorizationTokenType;
use iggyvolz\discordsocial\Client;
use iggyvolz\discordsocial\GetTokenResult;
use iggyvolz\discordsocial\Status;
use iggyvolz\discordsocial\StatusChangedEvent;
use League\Event\EventDispatcher;
use Monolog\Logger;
use Psr\EventDispatcher\EventDispatcherInterface;
use Psr\Log\LoggerInterface;
use Revolt\EventLoop;
use Monolog\Level;
use Monolog\Handler\StreamHandler;

require_once __DIR__ . "/vendor/autoload.php";

define("CLIENT_ID", intval(trim(file_get_contents(__DIR__ . "/application_id.txt"))));

// create a log channel
$log = new Logger('name');
$log->pushHandler(new StreamHandler('php://stdout', Level::Debug));
$eventDispatcher = new EventDispatcher();
$debugEventDispatcher = new readonly class($log, $eventDispatcher) implements EventDispatcherInterface {
    public function __construct(
        private LoggerInterface $log,
        private EventDispatcherInterface $eventDispatcher,
    ) {}

    public function dispatch(object $event): void
    {
        $this->log->debug("Dispatching " . get_class($event) . ": " . json_encode($event));
        $this->eventDispatcher->dispatch($event);
    }
};
$client = new iggyvolz\discordsocial\Client($log, $debugEventDispatcher);
$client->applicationId = CLIENT_ID;

EventLoop::repeat(0.1, fn() => Client::runCallbacks());
final readonly class TokenState implements JsonSerializable {
    public function __construct(
        public AuthorizationTokenType $tokenType,
        public string $accessToken,
    ) {}

    public static function from(GetTokenResult $getTokenResult) {
        return new self($getTokenResult->tokenType, $getTokenResult->accessToken);
    }

    public function jsonSerialize(): mixed
    {
        return ["tokenType" => $this->tokenType->value, "accessToken" => $this->accessToken];
    }
}
EventLoop::defer(function() use($log, $client){
    if(file_exists(__DIR__ . "/token.json")) {
        $tokenState = new MapperBuilder()->mapper()->map(TokenState::class, new JsonSource(file_get_contents(__DIR__ . "/token.json")));
    } else {
        $codeVerifier = $client->createAuthorizationCodeVerifier();
        $codeChallenge = $codeVerifier->challenge;
        $authorizeResult = $client->authorize(CLIENT_ID, $client->DefaultPresenceScopes, $codeChallenge)->await();
        $log->info("Received authorization code, exchanging for token");
        $getTokenResult = $client->getToken(CLIENT_ID, $authorizeResult->code, $codeVerifier->verifier, $authorizeResult->redirectUri)->await();
        $tokenState = TokenState::from($getTokenResult);
        file_put_contents(__DIR__ . "/token.json", json_encode($tokenState));
    }
    $client->updateToken($tokenState->tokenType, $tokenState->accessToken)->await();
    $client->connect();
});
$eventDispatcher->subscribeTo(StatusChangedEvent::class, function(StatusChangedEvent $event) use($client): void {
    if($event->status === Status::Connected) {
        $activity = new Activity();
        $activity->name = "foo";
        $activity->applicationId = CLIENT_ID;
        $client->updateRichPresence($activity)->await();
    }
});
EventLoop::run();
