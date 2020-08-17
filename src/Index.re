[@bs.val] [@bs.scope ("module")]
// set isHotEnabled equal to module.hot
external isHotEnabled : bool = "hot";
// set hotAccept equal to module.hot.accept
[@bs.val] [@bs.scope ("module", "hot")] external hotAccept : unit => unit = "accept";

ReactDOMRe.renderToElementWithId(
  <h1>{React.string("Hello World")}</h1>,
  "root"
);

// enable HMR
if (isHotEnabled) {
  hotAccept();
}
