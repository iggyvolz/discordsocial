<?php

namespace iggyvolz\discordsocial;

use Psr\EventDispatcher\EventDispatcherInterface;

class NullEventDispatcher implements EventDispatcherInterface{
    public function dispatch(object $event)
    {
    }
}