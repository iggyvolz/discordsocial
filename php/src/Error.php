<?php

namespace iggyvolz\discordsocial;

enum Error : int {
    case None = 0;
    case ConnectionFailed = 1;
    case UnexpectedClose = 2;
    case ConnectionCanceled = 3;
}