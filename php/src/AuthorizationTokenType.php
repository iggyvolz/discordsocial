<?php

namespace iggyvolz\discordsocial;

enum AuthorizationTokenType: int {
    case User = 0;
    case Bearer = 1;
}