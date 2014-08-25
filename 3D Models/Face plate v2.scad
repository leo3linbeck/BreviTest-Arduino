// front
difference() {
	cube([20, 125, 60]);
	// power switch
	translate([-0.5, 95, 25]) cube([21, 22, 28]);
	// power plug
	translate([-0.5, 106, 14]) rotate([0, 90, 0]) cylinder(r = 6, h = 21);
	// D sub 25 pin female connector
	translate([-0.5, 75, 10]) cube([21, 11, 41]);
	translate([-0.5, 80.5, 54]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);
	translate([-0.5, 80.5, 7]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);
	// arduino usb port
	translate([-0.5, 15, 12]) cube([21, 12, 11]);

	translate([2, 0, 0]) cube([20, 65, 30]);
	translate([-2, 0, 32]) cube([20, 125, 40]);
	translate([-2, 67, 0]) cube([20, 125, 32]);
}

difference() {
	translate([-80, 0, 0]) cube([100, 125, 2]);
	// arduino holes
	translate([-17, 9, 0]) cylinder(r = 1.6, h = 21);
	translate([-15, 51, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 22, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 50, 0]) cylinder(r = 1.6, h = 21);
	// power converter holes
	translate([-22, 85, 0]) cylinder(r = 1.6, h = 21);
	translate([-22, 115, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 85, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 115, 0]) cylinder(r = 1.6, h = 21);
	// power block holes
	translate([-40, 67, 0]) cylinder(r = 1.6, h = 21);
	translate([-48, 67, 0]) cylinder(r = 1.6, h = 21);
	translate([-56, 67, 0]) cylinder(r = 1.6, h = 21);
}