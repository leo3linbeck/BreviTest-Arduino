$fn = 50;

// front
difference() {
	translate([-1, 0, 0]) cube([21, 125, 60]);
	// power switch
	translate([-0.5, 95, 25]) cube([21, 22, 28]);
	// power plug
	translate([-0.5, 106, 14]) rotate([0, 90, 0]) cylinder(r = 6.3, h = 21);
	// D sub 25 pin female connector
	translate([-0.5, 75, 10]) cube([21, 11, 41]);
	translate([-0.5, 80.5, 54]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);
	translate([-0.5, 80.5, 7]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);
	// arduino usb port
	translate([-5, 15, 12]) cube([30, 12, 11]);

	translate([4, 5, 0]) cube([18, 32, 26]);
	translate([-2, 0, 28]) cube([20, 125, 44]);
	translate([-2, 67, 0]) cube([20, 125, 32]);
}

difference() {
	translate([-80, 0, 0]) cube([100, 125, 2]);
	// arduino holes
	translate([-68, 23, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 51, 0]) cylinder(r = 1.6, h = 21);
	// power converter holes
	translate([-22, 80, 0]) cylinder(r = 1.6, h = 21);
	translate([-22, 110, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 80, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 110, 0]) cylinder(r = 1.6, h = 21);
}

difference() {
	translate([-80, 61, 0]) cube([80, 6, 28]);
	// power block holes
	translate([-40, 70, 24]) rotate([90, 0, 0]) cylinder(r = 1.6, h = 21);
	translate([-48, 70, 24]) rotate([90, 0, 0]) cylinder(r = 1.6, h = 21);
	translate([-56, 70, 24]) rotate([90, 0, 0]) cylinder(r = 1.6, h = 21);
}

translate([-80, -4, 0]) cube([100, 4, 60]);
translate([-80, 122, 0]) cube([100, 3, 10]);
