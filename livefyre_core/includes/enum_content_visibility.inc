<?php
/**
 * Pseudo-enumeration for Livefyre content visibility.
 * 
 * http://docs.livefyre.com/developers/api-reference/
 */
abstract class ContentVisibility {
	const NONE = 0;
	const EVERYONE = 1;
	const OWNER = 2;
	const GROUP = 3;
}