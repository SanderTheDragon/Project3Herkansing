function inline() {
	$("img.svg").each(function(i, element) {
		const image = $(element);
		const source = image.attr("src");
		
		$.get(source, function(data) {
			let svg = $(data).find("svg");
			svg = svg.removeAttr("xmlns:a");
			
			image.replaceWith(svg);
		});
	});
}
