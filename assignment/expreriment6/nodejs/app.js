// var bezier = require(["adaptive-quadratic-curve"]);

var quadratic = require(["adaptive-quadratic-curve"]);
 
var start = [20, 20],
    c1 = [100, 159],
    end = [200, 20],
    scale = 2
 
var points = quadratic(start, c1, end, scale);
console.log(points);