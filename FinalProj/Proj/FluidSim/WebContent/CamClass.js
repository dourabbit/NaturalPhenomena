
var CamClass = (function () {
    // private static
    //var nextId = 1;

    // constructor
    var cls = function (name) {
        var name = name;
        var pMatrix = mat4.create();
        var invMatrix = mat4.create();
        var vMatrix = mat4.create();
        this.Initialize = function (gl) {

            gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
            gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

            mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

            mat4.identity(vMatrix);
            mat4.translate(vMatrix, [0, 0, 0]);
            mat4.inverse(vMatrix, vMatrix);
        }

        this.Update = function (shaderProgram) {
            gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
            gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

            //mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);

            gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
        }
        this.GetPersMatrix = function () {
            return pMatrix;
        };
        this.GetViewMatrix = function () {
            return vMatrix;
        };
    };


    // public static
    //    cls.get_nextId = function () {
    //        return nextId;
    //    };
    cls.prototype.GetRay = function (x, y, invMatrix, viewMatrix, point, direction) {
        var window_y = gl.viewportHeight - y - gl.viewportHeight / 2;
        var window_x = x - gl.viewportWidth / 2;
        var norm_x = window_x / (gl.viewportWidth / 2);
        var norm_y = window_y / (gl.viewportHeight / 2);

        //0.1=near Z
        var near = 0.1;
        var nearHeight = near * Math.tan(45 * Math.PI / 360.0);
        var aspect = gl.viewportWidth / gl.viewportHeight;

        var vecRay = [norm_x * nearHeight * aspect,
			norm_y * nearHeight,
			-near, 0];
        var vecPoint = [0, 0, -0.1, 1];
        vecPoint = mat4.multiplyVec4(viewMatrix, vecPoint);
        vecRay = mat4.multiplyVec4(invMatrix, vecRay);

        point[0] = vecPoint[0];
        point[1] = vecPoint[1];
        point[2] = vecPoint[2];

        var v = [0, 0, 0];
        v[0] = vecRay[0];
        v[1] = vecRay[1];
        v[2] = vecRay[2];

        var length = Math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

        direction[0] = v[0] / length;
        direction[1] = v[1] / length;
        direction[2] = v[2] / length;
    }





    //    // public (shared across instances)
    //    cls.prototype = {
    //        announce: function () {
    //            alert('Hi there! My id is ' + this.get_id() + ' and my name is "' + this.get_name() + '"!\r\n' +
    //                      'The next fellow\'s id will be ' + MyClass.get_nextId() + '!');
    //        }
    //    };

    return cls;
})();