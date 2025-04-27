<?php

namespace iggyvolz\discordsocial;

final readonly class StatusChangedEvent {
    public function __construct(
        public Status $status,
        public Error $error,
        public int $code,
    ) {}
}