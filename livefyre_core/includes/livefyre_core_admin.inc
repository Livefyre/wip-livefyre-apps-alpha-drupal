<?php
/**
 * @file
 * Controls all the admin functionality for the Livefyre Core.
 */
module_load_include('inc', 'livefyre_core', 'includes/constants');

function livefyre_core_menu() {
	$items = array();
	
	$items['admin/config/livefyre/livefyre_core'] = array(
		'title' => 'Livefyre Core',
		'description' => 'Core module and configuration for Livefyre functions.',
		'page callback' => 'drupal_get_form',
		'page arguments' => array('livefyre_core_form'),
		'access arguments' => array('administer users'),
		'type' => MENU_NORMAL_ITEM
	);
	
	return $items;
}

function livefyre_core_form($form, &$form_state) {
	$livefyreCoreApi = LivefyreCoreApi::getInstance();
	
	// Core Configuration
	$form['core_config'] = array(
		'#type' => 'fieldset',
		'#title' => t('Core Configuration'),
		'#description' => t("Configuration needed for all Livefyre actions."),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE
	);
	$form['core_config'][ENVIRONMENT] = array(
		'#type' => 'select',
		'#title' => t('Livefyre Environment'),
		'#options' => array(
			LivefyreEnvironment::PROD => t('Production'),
			LivefyreEnvironment::STAGING => t('Staging')
		),
		'#description' => t("Define which environment these credentials are for."),
		'#default_value' => $livefyreCoreApi->getEnvironment()
	);
	$form['core_config'][NETWORK] = array(
		'#type' => 'textfield',
		'#title' => t('Livefyre Network'),
		'#description' => t('e.g. example.fyre.co'),
		'#default_value' => $livefyreCoreApi->getNetwork(),
		'#required' => TRUE
	);
	$form['core_config'][SITE_ID] = array(
		'#type' => 'textfield',
		'#title' => t('Livefyre Site ID'),
		'#description' => t('e.g. 0123456789'),
		'#default_value' => $livefyreCoreApi->getSiteId(),
		'#required' => TRUE
	);
	$form['core_config'][NETWORK_KEY] = array(
		'#type' => 'textfield',
		'#title' => t('Livefyre Network Key'),
		'#description' => t('e.g. +VyZT0IByKbkuUvl5dKrIPt1Io='),
		'#default_value' => $livefyreCoreApi->getNetworkKey(),
		'#required' => TRUE
	);
	$form['core_config'][SITE_KEY] = array(
		'#type' => 'textfield',
		'#title' => t('Livefyre Site Key'),
		'#description' => t('e.g. +VyZT0IByKbkuUvl5dKrIPt1Io='),
		'#default_value' => $livefyreCoreApi->getSiteKey(),
		'#required' => TRUE
	);
	
	// Identity Configuration
	$form['identity_config'] = array(
		'#type' => 'fieldset',
		'#title' => t('Identity Configuration'),
		'#description' => t("Specify how your profile management system integrates with Livefyre's widgets"),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE
	);
	$form['identity_config'][LFEP_ENABLED] = array(
		'#type' => 'checkbox',
		'#title' => t("Use Livefyre Enterprise Profiles (Only enable if you have this service enabled)."),
		'#default_value' => $livefyreCoreApi->isLfepEnabled()
	);
	$form['identity_config'][AUTH_DELEGATES] = array(
		'#type' => 'textarea',
		'#title' => 'Authentication Delegates',
		'#description' => t("Define your authentication delegates in this field. Use JSON to define the login, logout, viewProfile, and editProfile delegates. For more details, see <a href='http://docs.livefyre.com/developers/identity-integration/your-identity/'>our docs</a>."),
		'#default_value' => $livefyreCoreApi->getAuthDelegates(),
		'#states' => array(
			'disabled' => array(
				'input[name="lf_lfep_enabled"]' => array('checked' => TRUE)
			)
		)
	);

	$form['#submit'][] = '_livefyre_core_form_sanitizer';
	return system_settings_form($form);
}

/**
 * Shim to strip the whitespaces off the ends of the user input-able fields.
 * Similar to system_settings_form_submit().
 */
function _livefyre_core_form_sanitizer($form, &$form_state) {
    $fields_to_clean = array('lf_network', 'lf_network_key', 'lf_site_id', 'lf_site_key');
    foreach($fields_to_clean as $k) {
        $form_state['values'][$k] = trim($form_state['values'][$k]);
    }
}