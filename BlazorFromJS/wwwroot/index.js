'use strict';

class Index {
    constructor(objRef, textArea) {
        new ResizeObserver(() => {
            objRef.invokeMethod('TextAreaResized', textArea.clientWidth, textArea.clientHeight);
        }).observe(textArea);
    }
}

export function createInstance(objRef, textArea) {
    return new Index(objRef, textArea);
}
