(function($) {
		let $disabledButton = {};

		let methods = {
			init: function (options) {
				options = jQuery.extend(
					{
						disabled: false, //true
						owerlay: false, //true
						input: false, //true
						center: false, //true
						position: "center",
						color:"#85a2b6",
						size:30,
						heightAuto: false,
						loader: 0,//[0,1,2]
						owerlayDiv: "loadBarOverlay"
					}, options
				);
				return this.each(function() {
					let targetLoad = $(this);
					let _id = (Math.random().toString(16) + "000000000").substr(2, 8);

					targetLoad.data("targetload", _id);

					let sizeLoader = options.size;
					let heightTarget = targetLoad.outerHeight();
					let widthTarget = targetLoad.outerWidth();

					if (options.input) {
						options.center = true;
						options.heightAuto = true;
					}

					if (widthTarget < heightTarget)
						options.heightAuto = false;

					if (options.heightAuto)
						sizeLoader = heightTarget;

					if (options.disabled) {
						if (options.input) {
							$disabledButton[_id] = $("input[type='submit'], button[type='submit']", targetLoad.closest("form"));
						}
						else {
							$disabledButton[_id] = targetLoad;
						}
					}
					if (options.owerlay) {
						$("#"+options.owerlayDiv).css({
							'position': 'absolute',
							'top': 0,
							'left': 0,
							'width': '100%',
							'height': '100%',
							'background-color': '#000',
							'opacity': '.5',
							'display': 'block',
							"z-index": maxZindex() + 1
						});
					}
					let inputWidth = 0;

					if (options.position === 'center')
						inputWidth = (widthTarget / 2) - (sizeLoader / 2);
					if (options.position === 'right')
						inputWidth = widthTarget - sizeLoader;
					if (options.position === 'left')
						inputWidth = widthTarget - sizeLoader;

					let $svgLoader = ['<svg version="1.1" id="svg_' + _id + '" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="40px" height="40px" viewBox="0 0 40 40" enable-background="new 0 0 40 40" xml:space="preserve">' +
					' <path opacity="0.2" fill="#85a2b6" d="M20.201,5.169c-8.254,0-14.946,6.692-14.946,14.946c0,8.255,6.692,14.946,14.946,14.946s14.946-6.691,14.946-14.946C35.146,11.861,28.455,5.169,20.201,5.169z M20.201,31.749c-6.425,0-11.634-5.208-11.634-11.634c0-6.425,5.209-11.634,11.634-11.634c6.425,0,11.633,5.209,11.633,11.634C31.834,26.541,26.626,31.749,20.201,31.749z"/>' +
					' <path fill="#85a2b6" d="M26.013,10.047l1.654-2.866c-2.198-1.272-4.743-2.012-7.466-2.012h0v3.312h0 C22.32,8.481,24.301,9.057,26.013,10.047z">' +
					' <animateTransform attributeType="xml"' +
					' attributeName="transform"' +
					' type="rotate"' +
					' from="0 20 20"' +
					' to="360 20 20"' +
					' dur="0.5s"' +
					' repeatCount="indefinite"/>' +
					' </path>' +
					' </svg>'
						,
						'<svg version="1.1" id="svg_' + _id + '" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="40px" height="40px" viewBox="0 0 50 50" enable-background="new 0 0 50 50" xml:space="preserve">' +
						'<path fill="#85a2b6" d="M25.251,6.461c-10.318,0-18.683,8.365-18.683,18.683h4.068c0-8.071,6.543-14.615,14.615-14.615V6.461z">' +
						' <animateTransform attributeType="xml"' +
						' attributeName="transform"' +
						' type="rotate"' +
						' from="0 25 25"' +
						' to="360 25 25"' +
						' dur="0.6s"' +
						' repeatCount="indefinite"/>' +
						' </path>' +
						' </svg>'
						,
						'<svg version="1.1" id="svg_' + _id + '" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="50px" height="50px" viewBox="0 0 50 50" enable-background="new 0 0 50 50" xml:space="preserve">' +
						' <path fill="#85a2b6" d="M43.935,25.145c0-10.318-8.364-18.683-18.683-18.683c-10.318,0-18.683,8.365-18.683,18.683h4.068c0-8.071,6.543-14.615,14.615-14.615c8.072,0,14.615,6.543,14.615,14.615H43.935z">' +
						' <animateTransform attributeType="xml"' +
						' attributeName="transform"' +
						' type="rotate"' +
						' from="0 25 25"' +
						' to="360 25 25"' +
						' dur="0.6s"' +
						' repeatCount="indefinite"/>' +
						' </path>' +
						' </svg>'];

					let $svg_loader = (Number(options.loader)< $svgLoader.length) ? $svgLoader[options.loader] : $svgLoader[0];

					function loadBarAjaxInit(svg_loader) {
						$('body').append('<div id="ajax_loader_' + _id + '" class="ajax-svg-loader" style="position: absolute; overflow: hidden; height: 40px; width: 40px; background: transparent; display: none;z-index: 1104;">' + svg_loader + '</div>');

						if($disabledButton[_id] != null)
							$disabledButton[_id].prop("disabled", true);

					}
					loadBarAjaxInit($svg_loader);

					function loadBarOverlayInit() {
						if (!$("#"+options.owerlayDiv).length) {
							$('body').append('<div id="'+options.owerlayDiv+'"></div>');
						}
					}
					if (options.owerlay)
						loadBarOverlayInit();

					function maxZindex() {
						return Math.max.apply(
							null, $.map(
								$('body > *'), function (e, n) {
									if ($(e).css('position') === 'absolute')
										return parseInt($(e).css('z-index')) || 1;
								}
							)
						);
					}

					let $tmpAjaxLoader = $("#ajax_loader_" + _id);

					function cssSetLoader(targetLoad, inputWidth, sizeLoader) {
						let positionLoader = $(targetLoad).offset();
						let topTarget = positionLoader.top;
						let leftTarget = positionLoader.left;

						let cssLeft = leftTarget + inputWidth;
						let cssTop = topTarget + (options.center ? (heightTarget - sizeLoader)/2 : 0);
						if (cssLeft < 0) {
							setTimeout(
								function () {
									cssSetLoader(targetLoad, inputWidth, sizeLoader);
								}, 500
							);
						}
						else {
							$tmpAjaxLoader.css({
								'top': cssTop,
								'left': cssLeft,
								'display': 'block',
								'z-index': maxZindex() + 10,
								'width': sizeLoader,
								'height': sizeLoader
							});
							$tmpAjaxLoader.find("#svg_" + _id).css({
								'width': sizeLoader,
								'height': sizeLoader
							});
							$tmpAjaxLoader.find("#svg_" + _id + " path").css('fill', options.color)
						}

						$(document).on('keyup',
							function(e) {
								if(e.keyCode === 27) {
									$('body').ajaxLoading('hide');
								}
							}
						);
					}

					cssSetLoader(targetLoad, inputWidth, sizeLoader);
				});
			},
			stop: function () {
				return this.each(function() {

					let targetLoad = $(this).data("targetload");
					if($disabledButton[targetLoad] != null){
						//console.log($disabledButton);
						$.each($disabledButton[targetLoad],function (i,e) {
							$(e).prop("disabled", false);
						});
						delete $disabledButton[targetLoad];
					}
					$("#ajax_loader_" + targetLoad).remove();
					$(document).off('keyup');
				});
			},
			hide: function () {
				return this.each(function() {
					$.each($disabledButton,function (i,e) {
						$(e).prop("disabled", false);
						delete $disabledButton[i];
					});
					$(".ajax-svg-loader").remove();
					$(document).off('keyup');
				});
			}
			/*show: function () {
			},
			update: function (content) {
			}*/
		};
		/**
		 * @author Sergey Mitskovets
		 *
		 * <b>Default:</b>
		 * $('#target').ajaxLoading();
		 *
		 * <b>Extended:</b>
		 * <b>disabled</b> - запрет отправки формы или блокировка самой кнопки
		 * <b>input</b>    - $('#target') - input
		 * <b>owerlay</b>  - background overlay
		 * <b>size</b>   - размер лоадера
		 * <b>center</b> - центрировать по вертикали
		 * <b>position</b> - позиция по горизонтали center, left, right
		 * <b>color</b> - цвет лоадера
		 * <b>heightAuto</b> - размер лоадера равен высоте $('#target')
		 *
		 * $('#target').ajaxLoading({size: 30, owerlay: true, disabled: true, input: true});
		 *
		 * @param {object} method {{init: init, stop: stop, hide: hide}}
		 */
		$.fn.ajaxLoading = function  (method ) {
			if ( methods[method] ) {
				return methods[method].apply( this, Array.prototype.slice.call( arguments, 1 ));
			} else if ( typeof method === 'object' || ! method ) {
				return methods.init.apply( this, arguments );
			} else {
				$.error( 'Метод ' +  method + ' не существует в jQuery.ajaxLoading' );
			}

		};
	}
)(jQuery);