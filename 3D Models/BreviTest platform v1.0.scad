$fn = 50;

render_platform = true;
render_stage = true;
render_teflon = false;
render_solenoid = false;

// all units in millimeters

solenoid_body_color = "Red";
// dimensions for solenoid at http://www.amazon.com/gp/product/B00DUYPQHU/ref=oh_aui_detailpage_o03_s00?ie=UTF8&psc=1
solenoid_body_height = 30;
solenoid_body_width = 25;
solenoid_body_length = 51;

solenoid_rod_color = "Black";
solenoid_stroke = 15;

platform_color = "LightBlue";
platform_height = 18;
platform_bevel_radius = 2;
platform_width = 100;
platform_length = 140;
platform_anchor_tolerance = 0.04;
platform_anchor_r = 3 + platform_anchor_tolerance; // half of d plus 0.04 tolerance
echo("platform_anchor_r = ", platform_anchor_r);
platform_anchor_rk = 9.78 / 2 + platform_anchor_tolerance; // half of dk plus 0.04 tolerance
echo("platform_anchor_rk = ", platform_anchor_rk);
platform_anchor_k = (5.7 + 0.3) * 2; // 0.3mm extra depth, and k is doubled for modeling
echo("platform_anchor_k = ", platform_anchor_k);
platform_anchor_spacing = 50;
platform_anchor_setback = 10;
platform_bumper_height = 8;
platform_bumper_width = 12;
platform_wiring_slot_length = 5;
platform_wiring_slot_width = 10;
platform_wiring_slot_margin = 4;

stage_color = "LightGreen";
stage_height = 35;
stage_bevel_radius = 2;
stage_width = 50;
stage_length = 50;

teflon_color = "LightGray";
teflon_pad_thickness = 0.8;
teflon_bottom_pad_length = stage_length + solenoid_stroke;
echo("teflon_bottom_pad_length = ", teflon_bottom_pad_length);

stage_rail_length = stage_length;
echo("stage_rail_length = ", stage_rail_length);
stage_rail_width = (platform_width - stage_width) / 4 - teflon_pad_thickness;
echo("stage_rail_width = ", stage_rail_width);
stage_rail_height = stage_height / 2 - teflon_pad_thickness;
echo("stage_rail_height = ", stage_rail_height);

platform_side_length = teflon_bottom_pad_length;
echo("platform_side_length = ", platform_side_length);
platform_side_height = stage_height + 2 * teflon_pad_thickness;
echo("platform_side_height = ", platform_side_height);
platform_side_width = stage_rail_width;
echo("platform_side_width = ", platform_side_width);

platform_rail_length = teflon_bottom_pad_length;
echo("platform_rail_length = ", platform_rail_length);
platform_rail_height = stage_height - stage_rail_height - 2 * teflon_pad_thickness;
echo("platform_rail_height = ", platform_rail_height);
platform_rail_width = (platform_width - stage_width) / 2 - platform_side_width - 2 * teflon_pad_thickness;
echo("platform_rail_width = ", platform_rail_width);

teflon_pad_width = stage_width + 2 * stage_rail_width;
echo("teflon_pad_width = ", teflon_pad_width);

solenoid_podium_length = platform_length - platform_rail_length;
echo("solenoid_podium_length = ", solenoid_podium_length);
solenoid_podium_width = solenoid_body_width + 10;
echo("solenoid_podium_width = ", solenoid_podium_width);
stage_center_of_mass_y = (stage_height * stage_width * stage_height / 2 + 2 * stage_rail_height * stage_rail_width * stage_rail_height / 2) / (stage_height * stage_width + 2 * stage_rail_height * stage_rail_width);
echo("stage_center_of_mass_y = ", stage_center_of_mass_y);
solenoid_podium_height = stage_center_of_mass_y - solenoid_body_height / 2;
echo("solenoid_podium_height = ", solenoid_podium_height);

if (render_platform) {
	// platform
	color(platform_color) {
		difference() {
			translate([0, 0, platform_height / 2])
				cube([platform_length, platform_width, platform_height], center = true);

	// anchor holes
			translate([platform_anchor_spacing + platform_anchor_setback, platform_anchor_spacing / 2, platform_height]) {
				cylinder(r = platform_anchor_r, h = 2 * platform_height, center = true);
				cylinder(r = platform_anchor_rk, h = platform_anchor_k, center = true);
			}
			translate([platform_anchor_setback, -platform_anchor_spacing / 2, platform_height]) {
				cylinder(r = platform_anchor_r, h = 2 * platform_height, center = true);
				cylinder(r = platform_anchor_rk, h = platform_anchor_k, center = true);
			}
			translate([platform_anchor_spacing + platform_anchor_setback, -platform_anchor_spacing / 2, platform_height]) {
				cylinder(r = platform_anchor_r, h = 2 * platform_height, center = true);
				cylinder(r = platform_anchor_rk, h = platform_anchor_k, center = true);
			}
			translate([platform_anchor_setback, platform_anchor_spacing / 2, platform_height]) {
				cylinder(r = platform_anchor_r, h = 2 * platform_height, center = true);
				cylinder(r = platform_anchor_rk, h = platform_anchor_k, center = true);
			}

	// wiring access slots
			translate([-(platform_length - platform_wiring_slot_length) / 2 + platform_wiring_slot_margin, (platform_width - platform_wiring_slot_width) / 2 - platform_wiring_slot_margin, 0]) {
				cube([platform_wiring_slot_length, platform_wiring_slot_width, 2 * platform_height], center = true);
			}

			translate([-(platform_length - platform_wiring_slot_length) / 2 + platform_wiring_slot_margin, -(platform_width - platform_wiring_slot_width) / 2 + platform_wiring_slot_margin, 0]) {
				cube([platform_wiring_slot_length, platform_wiring_slot_width, 2 * platform_height], center = true);
			}
		}
	}
	
	// platform rails
	color(platform_color) {
		translate([(platform_length - platform_rail_length) / 2,  (stage_width + stage_rail_width) / 2 + 2 * teflon_pad_thickness, platform_height + stage_height + 2 * teflon_pad_thickness - platform_rail_height / 2])
			cube([platform_rail_length, platform_rail_width, platform_rail_height], center = true);
	}
	
	color(platform_color) {
		translate([(platform_length - platform_rail_length) / 2,  -(stage_width + stage_rail_width) / 2 - 2 * teflon_pad_thickness, platform_height + stage_height + 2 * teflon_pad_thickness - platform_rail_height / 2])
			cube([platform_rail_length, platform_rail_width, platform_rail_height], center = true);
	}
	
	// platform sides
	
	color(platform_color) {
		translate([(platform_length - platform_rail_length) / 2,  (3 * stage_rail_width + stage_width) / 2 + 2 * teflon_pad_thickness, platform_height + teflon_pad_thickness + stage_height / 2])
			cube([platform_side_length, platform_side_width, platform_side_height], center = true);
	}
	
	color(platform_color) {
		translate([(platform_length - platform_rail_length) / 2,  -(3 * stage_rail_width + stage_width) / 2 - 2 * teflon_pad_thickness, platform_height + teflon_pad_thickness + stage_height / 2])
			cube([platform_side_length, platform_side_width, platform_side_height], center = true);
	}
	
	// platform stage bumper
	color(platform_color) {
		translate([-(platform_rail_length - platform_length / 2 + platform_bumper_width / 2), (platform_width + solenoid_podium_width) / 4, platform_height + platform_bumper_height / 2])
			cube([platform_bumper_width, (platform_width - solenoid_podium_width) / 2, platform_bumper_height], center = true);
	}
	
	color(platform_color) {
		translate([-(platform_rail_length - platform_length / 2 + platform_bumper_width / 2), -(platform_width + solenoid_podium_width) / 4, platform_height + platform_bumper_height / 2])
			cube([platform_bumper_width, (platform_width - solenoid_podium_width) / 2, platform_bumper_height], center = true);
	}
	
	// solenoid podium
	color(platform_color) {
		translate([-(platform_length - solenoid_podium_length) / 2, 0, platform_height + solenoid_podium_height / 2])
			cube([solenoid_podium_length, solenoid_podium_width, solenoid_podium_height], center = true);
	}
}

if (render_teflon) {
	// bottom teflon pad
	color(teflon_color) {
		translate([(platform_length - teflon_bottom_pad_length) / 2, 0, platform_height + teflon_pad_thickness / 2])
			cube([teflon_bottom_pad_length, teflon_pad_width, teflon_pad_thickness], center = true);
	}
	
	// top teflon pad
	color(teflon_color) {
		translate([(platform_length - stage_length) / 2, 0, platform_height + 3 * teflon_pad_thickness / 2])
			cube([stage_length, teflon_pad_width, teflon_pad_thickness], center = true);
	}
	
	// teflon on rails
	color(teflon_color) {
		translate([(platform_length - platform_rail_length) / 2,  (stage_width + stage_rail_width) / 2 + 2 * teflon_pad_thickness, platform_height + stage_rail_height + 3.5 * teflon_pad_thickness])
			cube([platform_rail_length, stage_rail_width, teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - stage_rail_length) / 2,  (stage_width + stage_rail_width) / 2, platform_height + stage_rail_height + 2.5 * teflon_pad_thickness])
			cube([stage_rail_length, stage_rail_width, teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - platform_rail_length) / 2,  -(stage_width + stage_rail_width) / 2 - 2 * teflon_pad_thickness, platform_height + stage_rail_height + 3.5 * teflon_pad_thickness])
			cube([platform_rail_length, stage_rail_width, teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - stage_rail_length) / 2,  -(stage_width + stage_rail_width) / 2, platform_height + stage_rail_height + 2.5 * teflon_pad_thickness])
			cube([stage_rail_length, stage_rail_width, teflon_pad_thickness], center = true);
	}
	
	// teflon on sides
	color(teflon_color) {
		translate([(platform_length - platform_rail_length) / 2,  stage_width / 2 + stage_rail_width + 3 * teflon_pad_thickness / 2, platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([platform_rail_length, teflon_pad_thickness, stage_rail_height + 2 * teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - stage_rail_length) / 2,  stage_width / 2 + stage_rail_width + teflon_pad_thickness / 2, platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([stage_rail_length, teflon_pad_thickness, stage_rail_height + 2 * teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - platform_rail_length) / 2,  -(stage_width / 2 + stage_rail_width + 3 * teflon_pad_thickness / 2), platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([platform_rail_length, teflon_pad_thickness, stage_rail_height + 2 * teflon_pad_thickness], center = true);
	}
	
	color(teflon_color) {
		translate([(platform_length - stage_rail_length) / 2,  -(stage_width / 2 + stage_rail_width + teflon_pad_thickness / 2), platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([stage_rail_length, teflon_pad_thickness, stage_rail_height + 2 * teflon_pad_thickness], center = true);
	}
}

if (render_stage) {
	// stage 
	color(stage_color) {
		difference() {
			translate([(platform_length - stage_length) / 2, 0, platform_height + stage_height / 2 + 2 * teflon_pad_thickness])
				cube([stage_length, stage_width, stage_height], center = true);
	// solenoid rod hole
			translate([0, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
				rotate([0, 90, 0]) cylinder(r = 0.7, h = 16, center = true);
		}
	}
	
	// stage rails
	
	color(stage_color) {
		translate([(platform_length - stage_length) / 2, (stage_width + stage_rail_width) / 2, platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([stage_length, stage_rail_width, stage_rail_height], center = true);
	}
	
	color(stage_color) {
		translate([(platform_length - stage_length) / 2, -(stage_width + stage_rail_width) / 2, platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([stage_length, stage_rail_width, stage_rail_height], center = true);
	}
}

if (render_solenoid) {
	// solenoid body
	color(solenoid_body_color) {
		translate([-(solenoid_body_length / 2 + solenoid_stroke) + platform_length / 2 - platform_side_length, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
			cube([solenoid_body_length, solenoid_body_width, solenoid_body_height], center = true);
	}
	
	// solenoid rod
	color(solenoid_rod_color) {
		translate([solenoid_stroke / 2 - solenoid_stroke, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
		rotate([0, 90, 0])
			cylinder(r = 1, h = solenoid_stroke, center = true);
	}
}



