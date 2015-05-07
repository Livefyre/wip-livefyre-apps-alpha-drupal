<?php
/**
 * Pseudo-enumeration for Livefyre app types.
 */
abstract class LivefyreAppType {
	const NONE = 0;
	
	// Core
	const COMMENTS = 1;
	const CHAT = 2;
	const LIVE_BLOG = 3;
	const REVIEWS = 4;
	const SIDENOTES = 5;
	
	// SDK
	const LIST_FEED = 100;
	const MEDIA_WALL = 101;
}