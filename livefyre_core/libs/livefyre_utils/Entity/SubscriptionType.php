<?php
require_once(dirname(__FILE__) . '/../Utils/BasicEnum.php');

abstract class SubscriptionType extends BasicEnum {
    const personalStream = 1;
}
