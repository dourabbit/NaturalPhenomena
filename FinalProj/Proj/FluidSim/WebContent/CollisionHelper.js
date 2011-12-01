
var CollisionHelper = (function () {
    // private static
    //var nextId = 1;

    // constructor
    var cls = function () {

    };

    cls.prototype.RayBox = function (ray, AABBCenter, AABBRadius) {
    };

    cls.prototype.RayPlane = function (ray, AABBCenter, AABBRadius) {
    };



    //    // public (shared across instances)
    //    cls.prototype = {
    //        announce: function () {
    //            alert('Hi there! My id is ' + this.get_id() + ' and my name is "' + this.get_name() + '"!\r\n' +
    //                      'The next fellow\'s id will be ' + MyClass.get_nextId() + '!');
    //        }
    //    };

    return cls;
})();




var Ray = (function () {
    var cls = function (point, direction) {
        var point = point;
        var direction = direction;




        this.direction = function () {
            return direction;
        };
        this.point = function () {
            return point;
        };
    };

    cls.prototype.RayPlane = function (plane, pointResult) {
        //t = -(Pn*R0+D)/(Pn*Rd)
        //Pn = planenormal
        //R0 = rayorigin
        //Rd = raydirection




        PnRd = plane.normal()[0] * this.direction()[0] + plane.normal()[1] * this.direction()[1] + plane.normal()[2] * this.direction()[2];
        PnR0 = -(plane.normal()[0] * this.point()[0] + plane.normal()[1] * this.point()[1] + plane.normal()[2] * this.point()[2] + plane.GetD());
        if (PnRd == 0) //no intersection or ray in the plane
            return false;
        else if (PnRd > 0)//the ray and plane pointing same direction
            return false;
        else {

            t = PnR0 /PnRd ;
            if (t < 0)//intersection behind origin
                return false;

            alert(plane.GetD());

            pointResult[0] = this.point()[0] + this.direction()[0] * t;
            pointResult[1] = this.point()[1] + this.direction()[1] * t;
            pointResult[2] = this.point()[2] + this.direction()[2] * t;
            return true;
        };



    };
    return cls;
})();


var Plane = (function () {

    var cls = function (point, normal) {
        var normal = normal;
        var point = point;
        var A = normal[0];
        var B = normal[1];
        var C = normal[2];
        var D = -(A * point[0] + B * point[1] + C * point[2]);

        this.GetD = function () {
            return D;
        };
        this.normal = function(){
            return normal;
        };
    };


    return cls;
})();