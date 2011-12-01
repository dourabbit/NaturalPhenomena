




var InputHelper = (function () {
    // private static
    //var nextId = 1;
    var Singleton;

    // constructor
    var cls = function (canvas) {
        var mouseEvents = {};
        var objectPool = [];


        this.handleMouseDown = function (event) {
            var point = [0, 0, 0];
            var vec = [0, 0, 0];
            Cam.GetRay(event.clientX, event.clientY, point, vec);

        }

        this.handleMouseUp = function (event) {
            if (event.identifier == null) {
                event.identifier = 0;
            }

            objectPool.push(mouseEvents[event.identifier]);
            mouseEvents[event.identifier] = null;
            cloth.release(event.identifier);
        }

        this.handleMouseMove = function (event) {

            var point = [0, 0, 0];
            var vec = [0, 0, 0];
            //Cam.GetRay(mouseEvents[event.identifier].lastMouseX, mouseEvents[event.identifier].lastMouseY, point, vec);
            document.frmOne.mouseX.value = event.clientX;
            document.frmOne.mouseY.value = event.clientY;
            //            cloth.move(event.identifier,
            //		            point[0] - (vec[0] * point[2] / vec[2]),
            //		            point[1] - (vec[1] * point[2] / vec[2]),
            //		            0);

            //mouseEvents[event.identifier].lastMouseX = event.clientX;
            //mouseEvents[event.identifier].lastMouseY = event.clientY;
        }
        Singleton = cls;




        canvas.onmousedown = this.handleMouseDown;
        canvas.onmouseup = this.handleMouseUp;
        canvas.onmousemove = this.handleMouseMove;
    };


    cls.GetInputHelper = function () {
        return Singleton;
    }

    return cls;
})();

