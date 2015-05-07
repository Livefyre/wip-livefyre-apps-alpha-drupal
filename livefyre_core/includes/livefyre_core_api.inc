<?php
module_load_include('php', 'livefyre_core', 'libs/livefyre_utils/Livefyre');
module_load_include('inc', 'livefyre_core', 'includes/constants');

class LivefyreCoreApi {
    private static $instance = NULL;
    private static $utilInstance = NULL;
    
    // Livefyre Core
    private $network;
    private $siteId;
    private $networkKey;
    private $siteKey;
    private $environment;
    
    // Identity/Profiles
    private $lfepEnabled;
    private $authDelegates;
    
    // Necessary for making singletons
    private function __construct() {}
    private function __clone() {}
    private function __wakeup() {}
    
    /**
     * Get an instance of the LF Util class
     * 
     * @return LivefyreUtilInstance
     */
    public function getUtilInstance() {
        if (self::$utilInstance === NULL) {
            $instance = self::getInstance();
            if ($instance->getNetwork() &&
                $instance->getNetworkKey()) {
                    self::$utilInstance = Livefyre::getNetwork($instance->getNetwork(), $instance->getNetworkKey());
            }
        }

        return self::$utilInstance;
    }
    
    /**
     * Get an instance of this class.
     * 
     * @return LivefyreCoreApi
     */
    public static function getInstance() {
        if (self::$instance === NULL) {
            $instance = new self();
            $instance->network = variable_get(NETWORK, '');
            $instance->siteId = variable_get(SITE_ID, '');
            $instance->networkKey = variable_get(NETWORK_KEY, '');
            $instance->siteKey = variable_get(SITE_KEY, '');
            $instance->environment = variable_get(ENVIRONMENT, LivefyreEnvironment::PROD);
            $instance->lfepEnabled = variable_get(LFEP_ENABLED, '');
            $instance->authDelegates = variable_get(AUTH_DELEGATES, '');
    
            self::$instance = $instance;
        }

        return self::$instance;
    }
    
    /**
     * Getter for a Livefyre Network
     * 
     * @param boolean $stripped Whether or not to remove the '.fyre.co' suffix such that it's just a 
     * network name. Defaults to FALSE.
     * @return string
     */
    public function getNetwork($stripped = FALSE) {
        return $stripped ? str_replace('.fyre.co', '', $this->network) : $this->network;
    }
    
    /**
     * Getter for a Livefyre Site ID
     */
    public function getSiteId() {
        return $this->siteId;
    }
    
    /**
     * Getter for a Livefyre Network Key
     */
    public function getNetworkKey() {
        return $this->networkKey;
    }
    
    /**
     * Getter for a Livefyre Site Key
     */
    public function getSiteKey() {
        return $this->siteKey;
    }
    
    /**
     * Getter for the Livefyre environment. See enum_environment.inc
     * for the available values.
     */
    public function getEnvironment() {
        return $this->environment;
    }
    
    /**
     * Get the Livefyre Domain (for constructing URLs) depending on the
     * current set environment.
     * 
     * @return string
     */
    public function getEnvironmentAsDomain() {
        return $this->environment == LivefyreEnvironment::STAGING ? ENV_STAGING : ENV_PROD;
    }
    
    /**
     * Getter for if we think the LFEP option is enabled or not.
     */
    public function isLfepEnabled() {
        return $this->lfepEnabled;
    }
    
    /**
     * Returns the auth delegate field (if any).
     */
    public function getAuthDelegates() {
        return $this->authDelegates;
    }
    
    /**
     * Hit the Livefyre Hottest API endpoint and return the
     * hottest collections in your network.
     * 
     * @param array $options Options to pass to the API endpoint.
     * @param string $format How you want the response returned to you. See format_response
     * for formatting options.
     */
    public function getHeatIndex($options = array(), $format = 'data-object') {
        $utilInstance = $this->getUtilInstance();
        $url = 'https://' . $this->getNetwork(TRUE) . '.bootstrap.fyre.co/api/v3.0/hottest/';
        if (!empty($options)) {
            $url .= '?' . http_build_query($options); 
        }

        $req = Client::GET($url);
        $resp = $this->format_response($req, $format);
        
        if (empty($resp) || (isset($resp->status) && $resp->status == "error")) {
            return array();
        }
        return $resp->data;
    }
    
    /**
     * Generates the necessary URIs for LFEP resources - both the javascript and
     * Janrain Application Name.
     * @return multitype:string
     */
    public function generateLfepUris() {
        $js_uri_domain = $this->getNetwork(TRUE) . '.ep.' . $this->getEnvironmentAsDomain();
        $hash = base64_encode($js_uri_domain);

        return array(
            'js_uri' => '//' . $js_uri_domain . '/media/' . $hash . '/javascripts/customprofiles.js',
            'app_name' => $this->getNetwork(TRUE) . '.auth.fyre.co'
        );
    }
    
    /**
     * Pulls data down from the Livefyre Bootstrap API endpoint.
     * 
     * @param string $article_id Livefyre article id
     * @param string $offset Which bootstrap resource to get - defaults to 'head'.
     * @param string $format How you want the response returned to you. See format_response
     * for formatting options.
     * @return multitype
     */
    public function getBootstrapData($article_id, $offset = 'head', $format = 'data-object') {
        $cache_key = "bootstrap:$article_id:$offset:$format";
        $cache = cache_get($cache_key);
        $data = '';

        if (!$this->is_valid_cache($cache)) {
            $formattedEnv = $this->getEnvironmentAsDomain() == ENV_STAGING ? ENV_STAGING . '/' : '';
            $url = 'https://' . $this->getNetwork(TRUE) . '.bootstrap.fyre.co/bs3/' .
                   $formattedEnv . $this->getNetwork() . '/' . $this->getSiteId() . '/' .
                   base64_encode($article_id) . '/' . $offset . '.json';
            $req = Client::GET($url);
            $resp = $this->format_response($req, $format);
            
            if (empty($resp) || (isset($resp->status) && $resp->status == "error")) {
                return array();
            }
            
            $data = $resp;
            cache_set($cache_key, $data, 'cache', time() + 600);
        }
        else {
            $data = $cache->data;
        }

        return $data;
    }
    
    /**
     * Pulls down the comment counts for the supplied Livefyre article ids
     * 
     * @param array $article_ids An array of Livefyre article ids
     * @param string $format How you want the response returned to you. See format_response
     * for formatting options.
     * @return multitype
     */
    public function getCommentCounts($article_ids = array(), $format = 'data-object') {
        $aids_as_string = implode(',', $article_ids);
        $cache_key = "commentCounts:$aids_as_string:$format";
        $cache = cache_get($cache_key);
        $data = '';

        if (!$this->is_valid_cache($cache)) {
            $site_id = $this->getSiteId();
            $hash = base64_encode($site_id . ':' . $aids_as_string);
            $url = 'https://' . $this->getNetwork(TRUE) . '.bootstrap.fyre.co/api/v1.1/public/comments/ncomments/' .
                   $hash . '.json';
            
            $req = Client::GET($url);
            $resp = $this->format_response($req, $format);
    
            if (empty($resp) || !isset($resp->data) || !isset($resp->data->$site_id)) {
                return array();
            }
            
            $data = $resp->data->$site_id;
            cache_set($cache_key, $data, 'cache', time() + 120);
        }
        else {
            $data = $cache->data;
        }

        return $data;
    }
    
    /**
     * Pulls down the top voted content from the Top Voted API.
     * @param string $article_id Livefyre article id
     * @param string $format How you want the response returned to you. See format_response
     * for formatting options.
     */
    public function getTopVotedContent($article_id, $format = 'data-object') {
        $cache_key = "topContent:$article_id:$format";
        $cache = cache_get($cache_key);
        $data = '';

        if (!$this->is_valid_cache($cache)) {
            $url = 'https://' . $this->getNetwork(TRUE) . '.bootstrap.fyre.co/api/v3.0/site/' . $this->getSiteId() .
                   '/article/' . base64_encode($article_id) . '/top/votes/';

            $req = Client::GET($url);
            $resp = $this->format_response($req, $format);
            
            if (empty($resp) || !isset($resp->data)) {
                return array();
            }
            
            $data = $resp->data;
            cache_set($cache_key, $data, 'cache', time() + 120);
        }
        else {
            $data = $cache->data;
        }

        return $data;
    }
    
    /**
     * Utility function that helps format the http response into a desired/usable
     * format
     * 
     * @param unknown $req Request object
     * @param string $format How you want the data to be formatted. Options are 'data-object', 
     * 'data-json', 'request-object', 'request-json'. Data returns only the 'data'  portion of 
     * the response from the request, while 'request' returns the entire response object.
     * @return multitype
     */
    protected function format_response($req, $format) {
        $ret = NULL;
        switch ($format) {
            case 'data-object':
                $ret = json_decode($req->data);
                break;
            case 'data-json':
                $ret = $req->data;
                break;
            case 'request-object':
                $ret = json_decode($ret);
                break;
            case 'request-json':
                $ret = $req;
        }

        return $ret;
    }
    
    /**
     * Helper method to determine if the cache is still valid
     * 
     * @param unknown $cache Drupal cache object
     * @return boolean
     */
    public function is_valid_cache($cache) {
        if (empty($cache) || $cache->expire < time()) {
            return FALSE;
        }

        return TRUE;
    }
}