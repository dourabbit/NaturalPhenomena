
var GridCell = (function () {
    // constructor
    var cls = function (origin) {
        // private
        var gl;
        var gridBuffer;
        var density;
        var velocity;
        var velocityBuffer;
        var gridVertices;
        var velVertices;

        var leftBottom = new Vector3D(origin.X, origin.Y, 0);
        var rightBottom = new Vector3D(origin.X + 1, origin.Y, 0);
        var leftUp = new Vector3D(origin.X, origin.Y + 1, 0);
        var rightUp = new Vector3D(origin.X + 1, origin.Y + 1, 0);
        var isSelected = false;

        this.SetSelection = function (selected) {
            isSelected = selected;
        }
        this.IsSelected = function () {
            return isSelected;
        }


        this.Initialize = function (g) {

            density = 0;
            velocity = [0, 0.2, 0];

            gl = g;

            gridBuffer = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, gridBuffer);

            gridVertices = [
                            leftBottom.X, leftBottom.Y, leftBottom.Z,
                            leftUp.X, leftUp.Y, leftUp.Z,
                            leftUp.X, leftUp.Y, leftUp.Z,
                            rightUp.X, rightUp.Y, rightUp.Z,
                            rightUp.X, rightUp.Y, rightUp.Z,
                            rightBottom.X, rightBottom.Y, rightBottom.Z,
                            rightBottom.X, rightBottom.Y, rightBottom.Z,
                            leftBottom.X, leftBottom.Y, leftBottom.Z
                            ];
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(gridVertices), gl.STATIC_DRAW);


            gridBuffer.itemSize = 3;
            gridBuffer.numItems = 8;



            velocityBuffer = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, velocityBuffer);


            velVertices = [
                            (leftBottom.X + rightUp.X) / 2, (leftBottom.Y + rightUp.Y) / 2, (leftBottom.Z + rightUp.Z) / 2,
                            (leftBottom.X + rightUp.X) / 2 + velocity[0],
                            (leftBottom.Y + rightUp.Y) / 2 + velocity[1],
                            (leftBottom.Z + rightUp.Z) / 2 + velocity[2]
                            ];
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(velVertices), gl.DYNAMIC_DRAW);
            velocityBuffer.itemSize = 3;
            velocityBuffer.numItems = 2;

        };

        this.Update = function (shaderProgram, d, v) {
            density = d;
            velocity = v;
            velVertices = [
                            (leftBottom.X + rightUp.X) / 2, (leftBottom.Y + rightUp.Y) / 2, (leftBottom.Z + rightUp.Z) / 2,
                            (leftBottom.X + rightUp.X) / 2 + velocity[0],
                            (leftBottom.Y + rightUp.Y) / 2 + velocity[1],
                            (leftBottom.Z + rightUp.Z) / 2 + velocity[2]
                            ];
            
        }

        this.Draw = function () {

            gl.bindBuffer(gl.ARRAY_BUFFER, gridBuffer);
            gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, gridBuffer.itemSize, gl.FLOAT, false, 0, 0);
            var s = this.IsSelected();

            if (s)
                gl.uniform4fv(shaderProgram.uColor, [1, 0, 0, 1]);
            else
                gl.uniform4fv(shaderProgram.uColor, [1, 1, 1, 1]);
            gl.drawArrays(gl.LINES, 0, 2);
            //gl.drawArrays(gl.LINES, 2, 2);
            //gl.drawArrays(gl.LINES, 4, 2);
            //gl.drawArrays(gl.LINES, 6, 2);

        };
        this.DrawVelocity = function () {



            gl.bindBuffer(gl.ARRAY_BUFFER, velocityBuffer);
            gl.bufferSubData(gl.ARRAY_BUFFER, 0, new Float32Array(velVertices));
            gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, velocityBuffer.itemSize, gl.FLOAT, false, 0, 0);

            gl.drawArrays(gl.LINES, 0, 2);

        };

    };

    return cls;
})();


function Grids(type, N, center,cam) {
    var type = type;
    var N = N;
    var gl;
    var GridsInfo = new Array();
    var center = center;
    var gridBuffer;
    var cam=cam;

    var mMatrix = mat4.create();

    for (var r = 0; r < N; r++) {
        for (var c = 0; c < N; c++) {
            var origin = new Vector3D(c, r, 0);
            GridsInfo.push(new GridCell(origin));

        }
    }

    this.Initialize = function (g) {

        gl = g;
        for (var index = 0; index < GridsInfo.length; index++) {
            GridsInfo[index].Initialize(g);
        }
        var offset = N / 2;
        mat4.identity(mMatrix);
        mat4.translate(mMatrix, [-offset, -offset, -offset*2.5]);

    }



    this.Update = function (shaderProgram) {
        var mvMatrix = mat4.create();
        mat4.multiply(mMatrix, cam.GetViewMatrix(), mvMatrix);
        gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);
        shaderProgram.isSelected
        //Grid Update
        var d = [0, 0, 0];
        var v = [0.3, 0.3, 0];

        GridsInfo[0].SetSelection(true);
        GridsInfo[2].SetSelection(true);

        for (var index = 0; index < GridsInfo.length; index++) {
            GridsInfo[index].Update(shaderProgram,d, v);
        }

    };


    this.GetGridCell = function (ray) {

        var dir = ray.direction();
        var point = ray.point();

        var inverse = mMatrix;
        mat4.inverse(inverse);
        var dir4 = [dir[0], dir[1], dir[2], 1];
        var point4 = [point[0], point[1], point[2], 1];

        dir4 = mat4.multiplyVec4(inverse, dir4);
        point4 = mat4.multiplyVec4(inverse, point4);

        var planeNormal4 = [0, 0, 1, 1];
        planeNormal4 = mat4.multiplyVec4(mMatrix, planeNormal4);
        var planeNormal = [planeNormal4[0], planeNormal4[1], planeNormal4[2]];


        var plane = new plane([0, 0, 0], planeNormal);
        var intersection = [0, 0, 0];

        if (!ray.RayPlane(plane, intersection));
        return;

        if (intersection[0] > 0 && intersection[0] < N &&
            intersection[1] > 0 && intersection[1] < N) {

            var ix = Math.floor(intersection[0]);
            var iy = Math.floor(intersection[1]);

            

        }
    };

    this.Draw = function () {

        //Grid Base
        for (var index = 0; index < GridsInfo.length; index++) {
            GridsInfo[index].Draw();
        }


        //Grid Velocity
        for (var index = 0; index < GridsInfo.length; index++) {
            GridsInfo[index].DrawVelocity();
        }


    };
}
