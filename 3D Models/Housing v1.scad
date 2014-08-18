$fn = 50;

show_base = true;
show_cover = true;

if (show_base) {
	// bottom
	cube([120, 120, 2]);
	
	// back
	difference() {
		cube([2, 120, 50]);
		// 12 pin connector
		translate([-0.5, 60, 10]) cube([3, 22, 28]);
		// button
		translate([-0.5, 100, 30]) rotate([0, 90, 0]) cylinder(r = 8, h = 3);
		// power plug
		translate([-0.5, 35, 16]) rotate([0, 90, 0]) cylinder(r = 6, h = 3);
	}
	
	// extension left
	translate([0, -10, 20])
	cube([120, 2, 30]);
	
	// extension back
	translate([0, -10, 20])
	cube([2, 10, 30]);
	
	// extension bottom
	translate([0, -10, 20])
	cube([120, 12, 2]);
	
	// front bottom
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
	
	translate([116, 0, 40 + push_up])
	cube([2, 120, 10]);
	
	translate([2, 0, 40 + push_up])
	cube([2, 120, 10]);

	translate([118, 0, 0])
	cube([2, 120, 50]);

	translate([118, -10, 20])
	cube([2, 10, 30]);
	
}
