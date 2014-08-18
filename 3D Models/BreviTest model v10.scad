$fn = 50;

render_platform = false;
render_stage = true;
render_solenoid = false;
render_cartridge = false;

// all units in millimeters

solenoid_body_color = "Red";
// dimensions for solenoid at http://www.amazon.com/gp/product/B00DUYPQHU/ref=oh_aui_detailpage_o03_s00?ie=UTF8&psc=1
solenoid_body_height = 30;
solenoid_body_width = 25;
solenoid_body_length = 51;

solenoid_rod_color = "Black";
solenoid_stroke = 7.1;
solenoid_rod_min = 6;

platform_color = "LightBlue";
platform_height = 16;
platform_back_margin = 15;
platform_width = 80;
platform_length = 120;
platform_anchor_tolerance = 0.04;
platform_anchor_r = 3 + platform_anchor_tolerance; // half of d plus 0.04 tolerance
echo("platform_anchor_r = ", platform_anchor_r);
platform_anchor_rk = 9.78 / 2 + platform_anchor_tolerance; // half of dk plus 0.04 tolerance
echo("platform_anchor_rk = ", platform_anchor_rk);
platform_anchor_k = (5.7 + 0.3) * 2; // 0.3mm extra depth, and k is doubled for modeling
echo("platform_anchor_k = ", platform_anchor_k);
platform_anchor_spacing = 50;
platform_anchor_setback = -platform_anchor_spacing / 2;
platform_wiring_slot_length = 4;
platform_wiring_slot_width = 10;
platform_wiring_slot_margin = 4;

teflon_pad_thickness = 0.8;

stage_color = "LightGreen";
stage_elevation = 38;
stage_height = stage_elevation - platform_height - 2 * teflon_pad_thickness;
stage_bevel_radius = 2;
stage_width = 40;

platform_side_width = 10;
echo("platform_side_width = ", platform_side_width);
platform_side_length = platform_anchor_spacing - 2 * platform_anchor_rk;
echo("platform_side_length = ", platform_side_length);
platform_side_height = stage_elevation - platform_height;
echo("platform_side_height = ", platform_side_height);

stage_length = platform_side_length - solenoid_stroke;
echo("stage_length = ", stage_length);

platform_bumper_height = platform_side_height;
echo("platform_bumper_height = ", platform_bumper_height);
platform_bumper_length = 10;
echo("platform_bumper_length = ", platform_bumper_length);
platform_bumper_width = (platform_width - stage_width) / 2 - platform_side_width;
echo("platform_bumper_width = ", platform_bumper_width);

stage_rail_length = stage_length - platform_bumper_length - teflon_pad_thickness;
echo("stage_rail_length = ", stage_rail_length);
stage_rail_width = platform_width - 2 * platform_side_width - 3.5 * teflon_pad_thickness;
echo("stage_rail_width = ", stage_rail_width);
stage_rail_height = 0.5 * platform_side_height - 2 * teflon_pad_thickness;
echo("stage_rail_height = ", stage_rail_height);

platform_rail_width = platform_bumper_width - 2 * teflon_pad_thickness;
echo("platform_rail_width = ", platform_rail_width);
platform_rail_length = platform_side_length - platform_bumper_length;
echo("platform_rail_length = ", platform_rail_length);
platform_rail_height = 0.5 * platform_side_height - 2 * teflon_pad_thickness;
echo("platform_rail_height = ", platform_rail_height);

cartridge_well_width = 8;
cartridge_well_distance_from_centerline = 4;
cartridge_path_width = 1.5;
accessory_x_spacing = 16;
accessory_y_offset = 12;
accessory_x_offset = cartridge_well_width;
echo("accessory_x_offset = ", accessory_x_offset);

solenoid_margin = 0.5;
solenoid_podium_length = solenoid_body_length + 2 * solenoid_margin;
echo("solenoid_podium_length = ", solenoid_podium_length);
solenoid_podium_width = solenoid_body_width + 2 * solenoid_margin;
echo("solenoid_podium_width = ", solenoid_podium_width);

stage_center_of_mass_y = 2 * teflon_pad_thickness + (stage_height * stage_width * stage_length * (stage_height / 2) + 2 * stage_rail_height * (stage_rail_width - stage_width) * stage_rail_length * (stage_rail_height / 2)) / (stage_height * stage_width * stage_length + stage_rail_height * 2 * (stage_rail_width - stage_width) * stage_rail_length);
echo("stage_center_of_mass_y = ", stage_center_of_mass_y);
solenoid_podium_height = stage_center_of_mass_y - solenoid_body_height / 2;
echo("solenoid_podium_height = ", solenoid_podium_height);
echo("solenoid_elevation = ", platform_height + solenoid_podium_height + solenoid_body_height);

if (render_platform) {
	// platform
	color(platform_color) {
		difference() {
			translate([-(platform_length / 2 - platform_anchor_spacing / 2 - platform_back_margin), 0, platform_height / 2])
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
			translate([-(platform_length - platform_back_margin - platform_anchor_spacing / 2 - platform_wiring_slot_length / 2) + platform_wiring_slot_margin, (platform_width - platform_wiring_slot_width) / 2 - platform_wiring_slot_margin, 0]) {
				cube([platform_wiring_slot_length, platform_wiring_slot_width, 2 * platform_height], center = true);
			}

			translate([-(platform_length - platform_back_margin - platform_anchor_spacing / 2 - platform_wiring_slot_length / 2) + platform_wiring_slot_margin, -(platform_width - platform_wiring_slot_width) / 2 + platform_wiring_slot_margin, 0]) {
				cube([platform_wiring_slot_length, platform_wiring_slot_width, 2 * platform_height], center = true);
			}

			translate([platform_back_margin + platform_anchor_spacing / 2 - platform_wiring_slot_length / 2 - platform_wiring_slot_margin, 0, 0]) {
				cube([platform_wiring_slot_length, 2 * platform_wiring_slot_width, 2 * platform_height], center = true);
			}

			if (solenoid_podium_height < 0) {
				translate([-(platform_side_length + solenoid_podium_length) / 2 - solenoid_rod_min + solenoid_margin, 0, platform_height + solenoid_podium_height / 2])
					cube([solenoid_podium_length, solenoid_podium_width, -2 * solenoid_podium_height], center = true);
			}
		}
	}
	
	// platform sides
	color(platform_color) {
		translate([0,  (platform_width - platform_side_width) / 2, platform_height + platform_side_height / 2])
			cube([platform_side_length, platform_side_width, platform_side_height], center = true);
	}
	
	color(platform_color) {
		translate([0, -(platform_width - platform_side_width) / 2, platform_height + platform_side_height / 2])
			cube([platform_side_length, platform_side_width, platform_side_height], center = true);
	}
	
	// platform rails
	color(platform_color) {
		translate([-(platform_side_length - platform_rail_length) / 2 + platform_bumper_length, ((platform_width - platform_rail_width) / 2 - platform_side_width), platform_height + platform_side_height - platform_rail_height / 2])
			cube([platform_rail_length, platform_rail_width, platform_rail_height], center = true);
	}
	
	color(platform_color) {
		translate([-(platform_side_length - platform_rail_length) / 2 + platform_bumper_length,  -((platform_width - platform_rail_width) / 2 - platform_side_width), platform_height + platform_side_height - platform_rail_height / 2])
			cube([platform_rail_length, platform_rail_width, platform_rail_height], center = true);
	}
	
	// platform stage bumper
	color(platform_color) {
		translate([-(platform_side_length - platform_bumper_length) / 2, platform_width / 2 - platform_side_width - platform_rail_width / 2, platform_height + platform_bumper_height / 2])
			cube([platform_bumper_length, platform_rail_width, platform_bumper_height], center = true);
	}
	
	color(platform_color) {
		translate([-(platform_side_length - platform_bumper_length) / 2, -(platform_width / 2 - platform_side_width - platform_rail_width / 2), platform_height + platform_bumper_height / 2])
			cube([platform_bumper_length, platform_rail_width, platform_bumper_height], center = true);
	}
	
	// solenoid podium
	if (solenoid_podium_height > 0) {
		color(platform_color) {
			translate([-(platform_side_length + solenoid_podium_length) / 2, 0, platform_height + solenoid_podium_height / 2])
				cube([solenoid_podium_length, solenoid_podium_width, solenoid_podium_height], center = true);
		}
	}
}

stage_x_offset = (stage_length - platform_side_length) / 2;
echo("stage_x_offset = ", stage_x_offset);
if (render_stage) {
	// stage 
	color(stage_color) {
		difference() {
			translate([stage_x_offset, 0, stage_elevation - stage_height / 2])
				cube([stage_length, stage_width, stage_height], center = true);

	// solenoid rod hole
			translate([-platform_side_length / 2, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
				rotate([0, 90, 0]) cylinder(r = 1.75, h = 30, center = true);

	// solenoid rod nut slot
			translate([4 - platform_side_length / 2, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2 - 3])
				cube([3, 6, 16], center = true);

	// accessory hole slots
	
				for (y = [-accessory_y_offset:accessory_y_offset:accessory_y_offset]) {
				translate([0, y, stage_elevation - 4])
					cube([46, 6, 3], center = true);
			}

	// accessory holes
			for (x = [-accessory_x_offset, accessory_x_spacing - accessory_x_offset]) {
				for (y = [-accessory_y_offset:accessory_y_offset:accessory_y_offset]) {
				translate([stage_x_offset + x, y, stage_elevation])
					cylinder(r = 1.5, h = 20, center = true);
			}
	
				for (y = [-accessory_y_offset:accessory_y_offset:accessory_y_offset]) {
				translate([stage_x_offset + x, y, stage_elevation])
					cylinder(r = 1.5, h = 20, center = true);
			}
		}
	}
}
	// stage rail
	
	color(stage_color) {
		translate([stage_x_offset + (stage_length - stage_rail_length) / 2, 0, platform_height + 2 * teflon_pad_thickness + stage_rail_height / 2])
			cube([stage_rail_length, stage_rail_width, stage_rail_height], center = true);
	}
}

if (render_solenoid) {
	// solenoid body
	color(solenoid_body_color) {
		translate([-(platform_side_length + solenoid_body_length) / 2 - solenoid_rod_min, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
			cube([solenoid_body_length, solenoid_body_width, solenoid_body_height], center = true);
	}
	
	// solenoid rod
	color(solenoid_rod_color) {
		translate([-(platform_side_length + solenoid_stroke) / 2, 0, platform_height + solenoid_podium_height + solenoid_body_height / 2])
		rotate([0, 90, 0])
			cylinder(r = 1, h = solenoid_stroke, center = true);
	}
}

p_y = 1.5; // path width
p_x = 4; // path length
w_x = 6; // well length
w_y = 8; // well width
n = 8; // number of wells 
m = 8; //separation from midline
	
module base_card()
{translate([-5,-20])square([100,40]);}

module corner(f)
{difference(){square([f,f]);circle(r=f,$fn=50);}}

module path()
{union(){
	translate([w_x/2,-w_y/2])square([p_x,p_y]); // actual path
	translate([w_x/2+1,-w_y/2+p_y+1])rotate(180)corner(1);}}//rounded corners

module path2()
{union(){translate([2*w_x+1,-w_y/2])square([p_x,p_y]); // actual path
			 //translate([2*w_x+2,-p_y/2-1])rotate(90)corner(1);
	      translate([2*w_x+2,-w_y/2+1+p_y])rotate(180)corner(1);//rounded corners	
	      //translate([2*w_x+p_x,-p_y/2-1])corner(1);
	      translate([2*w_x+p_x,-w_y/2+1+p_y])rotate(270)corner(1);}}//rounded corners

module well()
{difference ()
	{	square([w_x,w_y], center=true); 
		translate([w_x/2-2,w_y/2-2])corner(2);
		translate([-(w_x/2-2),w_y/2-2])rotate(90)corner(2);
		//translate([-(w_x/2-2),-(w_y/2-2)])rotate(180)corner(2);
		//translate([w_x/2-2,-(w_y/2-2)])rotate(270)corner(2);
   }}

module color_well() // last well
{translate([n*(w_x+p_x)+1,0])
{difference ()
	{	square([w_y,w_y], center=true); 
		translate([w_y/2-2,w_y/2-2])corner(2);
		translate([-(w_y/2-2),w_y/2-2])rotate(90)corner(2);
		//translate([-(w_y/2-2),-(w_y/2-2)])rotate(180)corner(2);
		//translate([w_y/2-2,-(w_y/2-2)])rotate(270)corner(2);
   }}}

module sample_well() // first well
{union(){translate([0,-w_y/2-1])square([2*w_x+1,w_y+2]); path2();}}

module control_well() // first well
{union(){translate([p_x+w_x,0])well(); path2();}}

module set(){union(){color_well(); 
	{for (i =[2:(n-1)]){translate([i*(p_x+w_x),0]){well();path();}}} }} // 1 set of wells, paths and comb
	
if (render_cartridge) {
	translate([0, -20, 45]) rotate(90, 0, 0) linear_extrude(height = 2, center = true)
	difference(){base_card(); 
						translate([0,-m]){sample_well(); set(); square([12,6],center=true);} 
						translate([0,m]){control_well(); set();}}
}





