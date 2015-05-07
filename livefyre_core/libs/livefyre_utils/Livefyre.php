<?php
require_once(dirname(__FILE__) . '/Core/Network.php');

class Livefyre { 
	public static function getNetwork($networkName, $networkKey) { 
		return new Network($networkName, $networkKey);
	}
}
