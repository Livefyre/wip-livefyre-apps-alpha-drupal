<?php if (!empty($title)): ?>
<div class="core-app-title"><?php echo $title; ?></div>
<?php endif; ?>

<?php if (!empty($app)): ?>
<script type="text/javascript">
(function ($) {
	var config = {
		network: '<?php echo $network; ?>',
		siteId: <?php echo $site_id; ?>,
		articleId: '<?php echo $article_id; ?>',
		collectionMeta: '<?php echo $collection_meta; ?>',
		checksum: '<?php echo $checksum; ?>'
	};

	<?php
	// Sidenotes
	if (strpos($app, 'sidenotes') !== FALSE):
	?>
		config['selectors'] = '<?php echo $selectors; ?>';
	<?php
	// All other apps - elseif just in case it's not defined
	elseif (!empty($el)):
	?>
		config['el'] = '<?php echo $el; ?>';
	<?php
	endif;
	
	if (!empty($environment)):
	?>
		config['environment'] = '<?php echo $environment; ?>';
	<?php
	endif;

	// Additional config
	if (!empty($json_config)):
	?>
	var addl_config = <?php echo $json_config; ?>;
	$.extend(config, addl_config);
	<?php
	endif;
	?>

	Livefyre.require(['auth'<?php echo ", '" . $app . "'"?><?php echo !is_null($auth_delegates['lfep_app_name']) ? ", 'lfep-auth-delegate#0'" : '';?>], function(Auth, App<?php echo !is_null($auth_delegates['lfep_app_name']) ? ', LFEPAuthDelegate' : '';?>) {
		new App(config);
		
		<?php 
		// LFEP auth delegate
		if(!is_null($auth_delegates['lfep_app_name'])) {
		?>
		var authDelegate = new LFEPAuthDelegate({
	        engageOpts: {
	            app: '<?php echo $auth_delegates['lfep_app_name']; ?>'
	        }
	    });
	    Auth.delegate(authDelegate);
		<?php
		}
		
		// All other auth delegates
		else {
		?>
		Auth.delegate(<?php echo $auth_delegates['custom']?>);
		<?php
		}
		?>
	});
})(jQuery);
</script>
<?php endif; ?>