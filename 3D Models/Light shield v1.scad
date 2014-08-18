$fn = 50;

difference() {
	cube([12, 12, 14]);	
	translate([2, 2, 0]) cube([8, 8, 14]);	
}

difference() {
	cube([12, 12, 2]);	
	translate([4, 6, 0]) cylinder(r = 1, h = 20);
	translate([8, 6, 0]) cylinder(r = 1, h = 20);
}

