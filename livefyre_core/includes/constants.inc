<?php
define('LF_PREFIX', 'lf_');

// LF Core
define('NETWORK', LF_PREFIX . 'network');
define('SITE_ID', LF_PREFIX . 'site_id');
define('NETWORK_KEY', LF_PREFIX . 'network_key');
define('SITE_KEY', LF_PREFIX . 'site_key');
define('ENVIRONMENT', LF_PREFIX . 'environment');

// Identity/Profile
define('LFEP_ENABLED', LF_PREFIX . 'lfep_enabled');
define('AUTH_DELEGATES', LF_PREFIX . 'auth_delegates');

// LF Articles
define('LF_MAX_ARTICLE_ID_LENGTH', 64);

// LF Environment Constants
define('ENV_STAGING', 't402.livefyre.com');
define('ENV_PROD', 'livefyre.com');