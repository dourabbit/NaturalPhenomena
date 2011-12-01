




var InputHelper = (function () {
    // private static
    //var nextId = 1;
    var threshold = 3;

    // constructor
    var cls = function (canvas, fncList) {
        //var mouseEvents = {};
        var objectPool;
        var fncs = fncList;
        //var oldX, oldY;
        var curEvent;
        this.handleMouseDown = function (event) {
            var point = [0, 0, 0];
            var vec = [0, 0, 0];
            //CamClass.GetRay(event.clientX, event.clientY, point, vec);
            //document.frmOne.mouseState.value = "MouseDown";
            //if(!objectPool)
            objectPool = [];
            if (!curEvent)
                curEvent = new Object();
            curEvent = { MousePosX: event.clientX, MousePosY: event.clientY, Event: "MouseDown" };
            objectPool.push(curEvent);
            //var events = objectPool;
            for (var i = 0; i < fncList.length; i++)
                fncList[i].call(InputHelper,objectPool);
        }

        this.handleMouseUp = function (event) {
            //            if (event.identifier == null) {
            //                event.identifier = 0;
            //            }

            //            objectPool.push(mouseEvents[event.identifier]);
            //            mouseEvents[event.identifier] = null;
            //            cloth.release(event.identifier);

            //document.frmOne.mouseState.value = "MouseUp";
            if (!curEvent)
                curEvent = new Object();
            curEvent = { MousePosX: event.clientX, MousePosY: event.clientY, Event: "MouseUp" };
            objectPool.push(curEvent);

            for (var i = 0; i < fncList.length; i++)
                fncList[i].call(objectPool);
        }

        this.handleMouseMove = function (event) {

            if (objectPool == undefined)
                return;
            if (objectPool.length == 0)
                return;
            if (objectPool[objectPool.length - 1].Event == "MouseUp") {
                return;
            }

            curEvent = { MousePosX: event.clientX, MousePosY: event.clientY, Event: "MouseMove" };
            objectPool.push(curEvent);
            for (var i = 0; i < fncList.length; i++)
                fncList[i].call(objectPool);

            //            var point = [0, 0, 0];
            //            var vec = [0, 0, 0];
            //CamClass.GetRay(mouseEvents[event.identifier].lastMouseX, mouseEvents[event.identifier].lastMouseY, point, vec);
            //            document.frmOne.mouseX.value = event.clientX;
            //            document.frmOne.mouseY.value = event.clientY;
            //            document.frmOne.oldMouseX.value = this.oldX;
            //            document.frmOne.oldMouseY.value = this.oldY;
            //            cloth.move(event.identifier,
            //		            point[0] - (vec[0] * point[2] / vec[2]),
            //		            point[1] - (vec[1] * point[2] / vec[2]),
            //		            0);

            //mouseEvents[event.identifier].lastMouseX = event.clientX;
            //mouseEvents[event.identifier].lastMouseY = event.clientY;
            //             if(this.oldX!=event.clientX&&this.oldY!=event.clientY)
            //             {
            //            	//document.frmOne.mouseState.value = "MouseMove";
            //             }
            //            else if(Math.abs(this.oldX-event.clientX)<threshold&&Math.abs(this.oldY-event.clientY)<threshold)
            //            {
            //            	//document.frmOne.mouseState.value = "MouseHovering";
            //            }
            //this.oldX = event.clientX;
            //this.oldY = event.clientY;
        };

        this.GetInputList = function () {

            return objectPool;
        };




        canvas.onmousedown = this.handleMouseDown;
        canvas.onmouseup = this.handleMouseUp;
        canvas.onmousemove = this.handleMouseMove;
    };



    return cls;
})();

