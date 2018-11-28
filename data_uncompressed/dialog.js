(function ($) {
	/**
	 * @param options
	 * <pre>
	 *  title
	 *  content
	 *  closeIcon
	 *  classModal
	 *  buttons
	 *  fade
	 * </pre>
	 */
	$.fn.createModal = function (options) {
		let _id = (Math.random().toString(16) + "000000000").substr(2, 8);
		let defaults = {
			title: "",
			content: $("<p />"),
			data: {},
			closeIcon: false,
			id: _id,
			open: function () {},
			buttons: [],
			classModal: "modal-md" //"modal-sm", "modal-lg"
		};
		let settings = $.extend(true, {}, defaults, options);

		let $modal = $("<div />").attr("id", settings.id).attr("role", "dialog").addClass("modal fade").data(settings.data)
			.append($("<div />").addClass("modal-dialog").addClass(settings.classModal)
				.append($("<div />").addClass("modal-content")
					.append($("<div />").addClass("modal-header")
						.append($("<h4 />").addClass("modal-title").text(settings.title)))
					.append($("<div />").addClass("modal-body")
						.append(settings.content))
					.append($("<div />").addClass("modal-footer")
					)
				)
			);
		$modal.shown = false;
		$modal.dismiss = function () {
			if (!$modal.shown) {
				window.setTimeout(function () {
					$modal.dismiss();
				}, 50);
				return;
			}

			$modal.modal("hide");
			$modal.prev().remove();
			$modal.empty().remove();

			$("body").removeClass("modal-open");
		};

		if (settings.closeIcon)
			$modal.find(".modal-header").prepend($("<button />").attr("type", "button").addClass("close").html("&times;").on("click", function () {
				$modal.modal("hide")
			}));

		if (settings.title === "")
			$modal.find(".modal-header").addClass("hidden");

		let $footer = $modal.find(".modal-footer");
		if (settings.buttons.length <= 0) {
			$footer.remove();
		} else {
			for (let i = 0; i < settings.buttons.length; i++) {
				(function (btn) {
					$footer.prepend($("<button />").addClass("btn " + (btn.class !== undefined ? btn.class : "btn-default"))
						.attr("id", btn.id)
						.attr("type", "button")
						.text(btn.text)
						.on("click", btn.click)
					)
				})(settings.buttons[i]);
			}
		}
		settings.open($modal);

		$modal.on('shown.bs.modal', function (e) {
			$modal.shown = true;

			if (settings.fade) {
				setTimeout(function () {
					$modal.modal("hide");
				}, 2000);
			}
		});
		$modal.on('hidden.bs.modal', function (e) {
			$modal.shown = false;
			$modal.remove();
		});
		$modal.modal("show");

		return $modal;
	};
})(jQuery);
(function ($) {
	/**
	 *
	 * @param title - заголовок окна
	 * @param message - сообщение окна
	 * @param classModal - класс (маленький, средний, большой) "modal-sm", "modal-md", "modal-lg"
	 * @param fade - скрывать через время автоматически (да/нет)
	 * @param buttons - кнопка закрыть (есть/нет)
	 * @param option - дополнительные свойства
	 */
	$.fn.openModal = function (title, message, classModal, fade, buttons, option) {
		let _id = (Math.random().toString(16) + "000000000").substr(2, 8);
		let _option = {};
		if (option !== undefined && option !== null && typeof option === "object") {
			_option = option;
		}
		let _buttons = (buttons !== false ? [
			{
				text: "Закрыть", class: "btn-primary", id: _id, click: function () {
					$(this).closest(".modal").modal("hide");
				}
			}
		] : []);

		if (buttons !== true && buttons !== null && typeof buttons === "object")
			_buttons.push(buttons);

		if (_option.buttons === "replace") {
			_buttons = buttons;
		}

		return $.fn.createModal({
			id: (_option.hasOwnProperty("id") ? _option["id"] : _id),
			title: title,
			data: (_option.hasOwnProperty("data") ? _option["data"] : {}),
			content: $("<div />").html(message),
			closeIcon: (_option.hasOwnProperty("closeIcon") ? _option["closeIcon"] : true),
			buttons: _buttons,
			classModal: (classModal !== "" ? classModal : "modal-md"),
			fade: fade
		});
	};

})(jQuery);