<?php
class Client {
	public static function GET($url, $headers = array()) {
		return drupal_http_request($url, array('headers' => $headers));
	}

	public static function POST($url, $headers = array(), $data = NULL) {
		return drupal_http_request($url, array(
				'data' => $data,
				'headers' => $headers,
				'method' => 'POST'
			)
		);
	}

	public static function PUT($url, $headers = array(), $data = NULL) {
		return drupal_http_request($url, array(
				'data' => $data,
				'headers' => $headers,
				'method' => 'PUT'
			)
		);
	}

	public static function DELETE($url, $headers = array(), $data = NULL) {
		return drupal_http_request($url, array(
				'data' => $data,
				'headers' => $headers,
				'method' => 'DELETE'
			)
		);
	}

	public static function PATCH($url, $headers = array(), $data = NULL) {
		return drupal_http_request($url, array(
				'data' => $data,
				'headers' => $headers,
				'method' => 'PATCH'
			)
		);
	}
}