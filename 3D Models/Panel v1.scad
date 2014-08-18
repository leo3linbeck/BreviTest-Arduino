$fn = 50;

// back
difference() {
	cube([60, 60, 2]);
	// 12 pin connector
	translate([8, 25, -0.5]) cube([22, 28, 3]);
	// button
	translate([45, 39, -0.5]) cylinder(r = 8, h = 3);
	// power plug
	translate([19, 12, -0.5]) cylinder(r = 6, h = 3);
}
