

import * as React from "react";
import * as ReactDOMRe from "../node_modules/reason-react/src/legacy/ReactDOMRe.bs.js";

ReactDOMRe.renderToElementWithId(React.createElement("h1", undefined, "Hello World"), "root");

if (module.hot) {
  module.hot.accept();
}

export {
  
}
/*  Not a pure module */
