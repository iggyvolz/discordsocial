<?php

namespace iggyvolz\discordsocial;

final class ClientResult {
    private function __construct() {}
    public function __destruct() {}
    public function __toString(): string {}
    public ErrorType $type {get {} set{}}
    public string $error {get {} set {}}
    public int $errorCode {get {} set {}}
    public HttpStatusCode $status {get {} set {}}
    public string $responseBody {get {} set {}}
    public bool $successful {get {} set {}}
    public bool $retryable {get {} set {}}
    public float $retryAfter {get {} set {}}
    public function __debugInfo(): ?array
    {
        return [
            "__toString" => $this->__toString(),
            "type" => $this->type,
            "error" => $this->error,
            "errorCode" => $this->errorCode,
            "status" => $this->status,
            "responseBody" => $this->responseBody,
            "successful" => $this->successful,
            "retryable" => $this->retryable,
            "retryAfter" => $this->retryAfter
        ];
    }
}