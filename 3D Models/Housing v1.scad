$fn = 50;

show_base = true;
show_cover = true;

if (show_base) {
	// bottom
	cube([120, 120, 2]);

	// bottom left bracket
	translate([0, 118, 2])
	difference() {
		cube([118, 2, 10]);
		translate([30, -0.5, 5]) rotate([0, 90, 90]) cylinder(r = 3, h = 3);
		translate([90, -0.5, 5]) rotate([0, 90, 90]) cylinder(r = 3, h = 3);
	}
	
	// bottom back bracket
	translate([116, 0, 2])
	difference() {
		cube([2, 118, 10]);
		translate([-0.5, 30, 5]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
		translate([-0.5, 90, 5]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
	}
	
	// front left bracket
	translate([2, 118, 0])
	difference() {
		cube([10, 2, 50]);
		translate([5, -0.5, 40]) rotate([0, 90, 90]) cylinder(r = 3, h = 3);
	}
	
	// back right bottom bracket
	translate([116, 2, 0])
	cube([2, 10, 22]);

	// back right shelf bracket
	translate([116, -10, 22])
	difference() {
		cube([2, 22, 10]);
		translate([-0.5, 17, 5]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
	}
	
	// back right top bracket
	translate([116, -8, 20])
	cube([2, 10, 30]);
	
	// front
	difference() {
		cube([2, 120, 50]);
		// power switch
		translate([-0.5, 60, 10]) cube([3, 22, 28]);
		// pushbutton
		translate([-0.5, 100, 30]) rotate([0, 90, 0]) cylinder(r = 8, h = 3);
		// power plug
		translate([-0.5, 35, 16]) rotate([0, 90, 0]) cylinder(r = 6, h = 3);
	}
	
	// extension right
	translate([0, -10, 20])
	difference() {
		cube([120, 2, 30]);
		translate([30, -0.5, 25]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
		translate([90, -0.5, 25]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
	}
	
	// extension front
	translate([0, -10, 20])
	cube([2, 10, 30]);
	
	// extension bottom
	translate([0, -10, 20])
	cube([120, 12, 2]);
	
	// right bottom
	difference() {
		cube([120, 2, 20]);
		// arduino usb
		translate([60, -0.5, 8]) cube([12, 3, 11]);
	}
}

push_up = 0;
if (show_cover) {
	// top
	translate([0, -10, 50 + push_up])
	cube([120, 130, 2]);
	
	// top right bracket
	translate([2, -8, 40 + push_up])
	difference() {
		cube([118, 2, 10]);
		translate([28, -0.5, 5]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
		translate([88, -0.5, 5]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
	}

	// top front bracket
	translate([2, -6, 40 + push_up])
	difference() {
		cube([2, 126, 10]);
		translate([-0.5, 26, 5]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
		translate([-0.5, 96, 5]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
	}

	// back
	translate([118, 0, 0])
	difference() {
		cube([2, 120, 50]);
		translate([-0.5, 90, 17]) cube([3, 17, 23]);
		translate([-0.5, 30, 7]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
		translate([-0.5, 90, 7]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
		translate([-0.5, 7, 27]) rotate([0, 90, 0]) cylinder(r = 3, h = 3);
	}

	// right top
	translate([118, -10, 20])
		cube([2, 10, 30]);
	
	// left
	translate([0, 120, 0])
	difference() {
		cube([120, 2, 52]);
		translate([30, -0.5, 7]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
		translate([90, -0.5, 7]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
		translate([7, -0.5, 40]) rotate([-90, 0, 0]) cylinder(r = 3, h = 3);
	}
}
