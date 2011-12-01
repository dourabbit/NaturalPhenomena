Vector3DClass = {
    Vector3D: function (x, y, z) {
        this.x = 0;
        this.y = 0;
        this.z = 0;

        this.reset(x, y, z);
    },
    reset: function (x, y, z) {
        this.x = x == undefined ? 0 : x;
        this.y = y == undefined ? 0 : y;
        this.z = z == undefined ? 0 : z;
    },
    clone: function () {
        return new flashgl.core.math.Vector3D(this.x, this.y, this.z);
    },
    copy: function (n) {
        this.x = n.x;
        this.y = n.y;
        this.z = n.z;
    },
    multiplyEq: function (n) {
        this.x *= n;
        this.y *= n;
        this.z *= n;
    },
    plusEq: function (v) {
        this.x += v.x;
        this.y += v.y;
        this.z += v.z;
    },
    minusEq: function (v) {
        this.x -= v.x;
        this.y -= v.y;
        this.z -= v.z;

    },
    normalize: function () {
        var mod = Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);

        if (mod != 0 && mod != 1) {
            mod = 1 / mod; // mults are cheaper then divs
            this.x *= mod;
            this.y *= mod;
            this.z *= mod;
        }
    },
    modulo: function () {
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    },
    moduloSquared: function () {
        return (this.x * this.x + this.y * this.y + this.z * this.z);
    },
    //************************************************* trace *************************************************
    toString: function () {
        return "Vector3D(" + this.x + ", " + this.y + ", " + this.z + ")";
    }
}

//********************************************************************************************************************
//                                                                                               ***** STATIC *****
//********************************************************************************************************************

Vector3DClass.Vector3D.toDEGREES = 180 / Math.PI;
Vector3DClass.Vector3D.toRADIANS = Math.PI / 180;

//************************************************* CREATE *************************************************

Vector3DClass.Vector3D.create = function (x, y, z) {
    return new flashgl.core.math.Vector3D(x, y, z);
}

Vector3DClass.Vector3D.ZERO = function () {
    return this.create(0, 0, 0);
}

//************************************************* OPERATIONS *************************************************

Vector3DClass.Vector3D.normal = function (v) {
    var vn = v.clone();
    vn.normalize();

    return vn;
}

Vector3DClass.Vector3D.add = function (v, w) {
    return this.create
        (
                v.x + w.x,
                v.y + w.y,
                v.z + w.z
        );
}

Vector3DClass.Vector3D.sub = function (v, w) {
    return this.create
        (
                v.x - w.x,
                v.y - w.y,
                v.z - w.z
        );
}

Vector3DClass.Vector3D.dot = function (v, w) {
    return (v.x * w.x + v.y * w.y + w.z * v.z);
}

Vector3DClass.Vector3D.cross = function (v, w, targetN) {
    if (targetN == undefined)
        targetN = this.ZERO();

    targetN.reset((w.y * v.z) - (w.z * v.y), (w.z * v.x) - (w.x * v.z), (w.x * v.y) - (w.y * v.x));
    return targetN;
}

//************************************************* ALIAS *************************************************

var $V3D = Vector3DClass.Vector3D.create;

Class.define("Vector3D", Vector3DClass); // Class name alias

//*************************************************************************************************************

Class.define("flashgl.core.math.Vector3D", Vector3DClass);