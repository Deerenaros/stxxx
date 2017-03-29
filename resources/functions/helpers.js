function create(path, attrs, parent, success) {
    var component = Qt.createComponent(path)
    if(component !== null && component.status === Component.Ready) {
        var o = component.createObject(parent, attrs);
        if(o !== null && success && {}.toString.call(success) === '[object Function]') {
            success(o);
        } return o;
    } else if(component !== null && component.status === Component.Error) {
        console.debug(component.errorString())
    }

    return null;
}
