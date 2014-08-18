$fn = 50;

// bottom
cube([50, 20, 2]);

// extension left
translate([0, -10, 20])
cube([50, 2, 20]);

// extension bottom
translate([0, -10, 20])
cube([50, 12, 2]);

// front bottom
difference() {
	cube([50, 2, 20]);
	// arduino usb
	translate([20, -0.5, 8]) cube([12, 3, 11]);
}

