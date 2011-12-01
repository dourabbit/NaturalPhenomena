
var Vector3D = (function () {
    // private static
    var nextId = 1;

    // constructor
    var cls = function (x, y, z) {

        this.X = x; this.Y = y; this.Z = z;

    };


    //    // public (shared across instances)
    //    cls.prototype = {
    //        announce: function () {
    //            alert('Hi there! My id is ' + this.get_id() + ' and my name is "' + this.get_name() + '"!\r\n' +
    //                'The next fellow\'s id will be ' + MyClass.get_nextId() + '!');
    //        }
    //    };

    return cls;
})();
