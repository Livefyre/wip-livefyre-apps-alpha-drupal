<?php
/**
 * Maps a piece of content to Livefyre's content source and returns a content source.
 * 
 * http://docs.livefyre.com/developers/api-reference/
 */
function getContentSource ($source_id) {
	$livefyre = array(0, 4, 5, 8);
	$twitter = array(1, 2, 7, 15);
	$facebook = array(3, 6, 14);
	$google_plus = array(10);
	$youtube = array(12, 16);
	$rss = array(13);
	$instagram = array(19);
	
	if (in_array($source_id, $livefyre)) {
		return ContentSource::LIVEFYRE;
	}
	elseif (in_array($source_id, $twitter)) {
		return ContentSource::TWITTER;
	}
	elseif (in_array($source_id, $facebook)) {
		return ContentSource::FACEBOOK;
	}
	elseif (in_array($source_id, $google_plus)) {
		return ContentSource::GOOGLE_PLUS;
	}
	elseif (in_array($source_id, $youtube)) {
		return ContentSource::YOUTUBE;
	}
	elseif (in_array($source_id, $rss)) {
		return ContentSource::RSS;
	}
	elseif (in_array($source_id, $instagram)) {
		return ContentSource::INSTAGRAM;
	}
}