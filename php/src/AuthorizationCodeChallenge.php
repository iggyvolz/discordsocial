<?php

namespace iggyvolz\discordsocial;

final class AuthorizationCodeChallenge {
    public function __destruct() {}
    public AuthenticationCodeChallengeMethod $method {get{} set{}}
    public string $challenge {get{} set{}}
    public function __debugInfo(): ?array
    {
        return ["method" => $this->method, "challenge" => $this->challenge];
    }
}