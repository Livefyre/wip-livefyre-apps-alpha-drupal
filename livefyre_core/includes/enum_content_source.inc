<?php
/**
 * Pseudo-enumeration for Livefyre content sources.
 * 
 * http://docs.livefyre.com/developers/api-reference/
 */
abstract class ContentSource {
	const UNKNOWN = 0;
	const LIVEFYRE = 1;
	const TWITTER = 2;
	const FACEBOOK = 3;
	const GOOGLE_PLUS = 4;
	const YOUTUBE = 5;
	const RSS = 6;
	const INSTAGRAM = 7;
}