<?php
require_once(dirname(__FILE__) . '/../Entity/TimelineCursor.php');

class CursorFactory {
	public static function getTopicStreamCursor($core, $topic, $limit = 50, $date = null) {
    	if (is_null($date)) {
    		$date = time();
    	}
		$resource = $topic->getId() . ":topicStream";
		return new TimelineCursor($core, $resource, $limit, $date);
	}

	public static function getPersonalStreamCursor($network, $user, $limit = 50, $date = null) {
    	if (is_null($date)) {
    		$date = time();
    	}
		$resource = $network->getUserUrn($user) . ":personalStream";
		return new TimelineCursor($network, $resource, $limit, $date);
	}
}
