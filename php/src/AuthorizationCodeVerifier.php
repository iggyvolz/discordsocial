<?php

namespace iggyvolz\discordsocial;

final class AuthorizationCodeVerifier {
    private function __construct() {}
    public function __destruct() {}
    public string $verifier {get{} set{}}
    public AuthorizationCodeChallenge $challenge {get{} set{}}
    public function __debugInfo(): ?array
    {
        return ["verifier" => $this->verifier, "challenge" => $this->challenge];
    }
}