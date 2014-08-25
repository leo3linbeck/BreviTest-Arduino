$fn = 50;

difference() {
	cube([2, 90, 30]);
	// D sub 25 pin female connector
	translate([-0.5, 8, 8]) rotate([0, 90, 0]) cylinder(r = 2, h = 3);
	translate([-0.5, 82, 8]) rotate([0, 90, 0]) cylinder(r = 2, h = 3);
	translate([0, -35, 0]) {
		translate([-0.5, 60, 10]) cube([3, 41, 11]);
		translate([-0.5, 57, 15]) rotate([0, 90, 0]) cylinder(r = 2, h = 3);
		translate([-0.5, 104, 15]) rotate([0, 90, 0]) cylinder(r = 2, h = 3);
	}
}

difference() {
	cube([15, 90, 2]);
	translate([7.5, 15, -0.5]) cylinder(r = 2, h = 3);
	translate([7.5, 75, -0.5]) cylinder(r = 2, h = 3);
}
