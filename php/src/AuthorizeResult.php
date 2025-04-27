<?php

namespace iggyvolz\discordsocial;

final readonly class AuthorizeResult {
    public function __construct(
        public string $code,
        public string $redirectUri
    ) {}
}