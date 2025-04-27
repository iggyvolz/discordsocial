<?php

namespace iggyvolz\discordsocial;

final readonly class GetTokenResult {
    public function __construct(
        public string $accessToken,
        public string $refreshToken,
        public AuthorizationTokenType $tokenType,
        public int $expiresIn,
        /**
         * @var list<string>
         */
        public array $scopes
    ) {}
}