<?php if (!empty($title)): ?>
<div class="sdk-app-title"><?php echo $title; ?></div>
<?php endif; ?>
<?php if (!empty($app)): ?>
<script type="text/javascript">
(function ($) {
	Livefyre.require(['streamhub-sdk#2.16.0'<?php echo ", '" . $app . "'"; ?>], function(SDK, App) {
		var config = {
			network: '<?php echo $network; ?>',
			siteId: <?php echo $site_id; ?>,
			articleId: '<?php echo $article_id; ?>'
		};

		<?php
		if (!empty($environment)): 
		?>
		config['environment'] = '<?php echo $environment; ?>';
		<?php
		endif;
		?>
		
		var app_config = {
			el: document.getElementById('<?php echo $el; ?>'),
			collection: new SDK.Collection(config)
		};
		<?php
		// Additional SDK config
		if (!empty($json_config)):
		?>
		var addl_config = <?php echo $json_config; ?>;
		$.extend(app_config, addl_config);
		<?php
		endif;
		?>

		var app = new App (app_config);
	});
})(jQuery);
</script>
<?php endif; ?>