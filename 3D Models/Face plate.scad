// front
difference() {
	cube([20, 120, 60]);
	// power switch
	translate([-0.5, 90, 25]) cube([21, 22, 28]);
	// power plug
	translate([-0.5, 101, 14]) rotate([0, 90, 0]) cylinder(r = 6, h = 21);
	// D sub 25 pin female connector
	translate([-0.5, 72, 10]) cube([21, 11, 41]);
	translate([-0.5, 77.5, 54]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);
	translate([-0.5, 77.5, 7]) rotate([0, 90, 0]) cylinder(r = 2, h = 21);

	translate([-0.5, 15, 12]) cube([21, 12, 11]);

	translate([2, 0, 0]) cube([20, 65, 30]);
	translate([-2, 0, 32]) cube([20, 120, 40]);
	translate([-2, 67, 0]) cube([20, 120, 32]);
}

difference() {
	translate([-80, 0, 0]) cube([100, 120, 2]);
	translate([-17, 9, 0]) cylinder(r = 1.6, h = 21);
	translate([-15, 51, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 18, 0]) cylinder(r = 1.6, h = 21);
	translate([-68, 46, 0]) cylinder(r = 1.6, h = 21);
	translate([-20, 80, 0]) cylinder(r = 1.6, h = 21);
	translate([-20, 110, 0]) cylinder(r = 1.6, h = 21);
	translate([-66, 80, 0]) cylinder(r = 1.6, h = 21);
	translate([-66, 110, 0]) cylinder(r = 1.6, h = 21);
}