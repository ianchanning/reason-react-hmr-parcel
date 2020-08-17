# Reason React with Hot Module Replacement

## What

This is a starter project for [Reason React](https://rescript-lang.org/docs/reason-react/latest/introduction) that makes use of the zero configuration [Parcel](https://parceljs.org) JavaScript bundler to provide compilation and [Hot Module Replacement (HMR) for React](https://gaearon.github.io/react-hot-loader/getstarted/).

## Quickstart

For yarn:

```sh
yarn install
yarn start
```

For npm (I only tested with yarn, let me know if this doesn't work):

```sh
npm install
npm run start
```

Goto <http://localhost:1234/> and you should see a "Hello World" page. If you modify `src/Index.re` then the page should update without refreshing.

## Why

This project started with the [ReasonReact official template](https://rescript-lang.org/docs/reason-react/latest/installation) created via

```sh
bsb -init my-react-app -theme react-hooks
```

However that uses a custom [self described 'hack'](https://github.com/marijnh/moduleserve) for a server and their own `watcher.js` code:

```html
  <!-- This is https://github.com/marijnh/moduleserve, the secret sauce that allows us not need to bundle things during development, and have instantaneous iteration feedback, without any hot-reloading or extra build pipeline needed. -->
  <script src="/moduleserve/load.js" data-module="/src/Index.bs.js"></script>
  <!-- Our little watcher. Super clean. Check it out! -->
  <script src="/watcher.js"></script>
```

I understand their desire to provide an alternative to Webpack and they do provide a Webpack config for 'production'. The `watcher.js` code is very interesting and powerful for just 30 lines of JS code but I couldn't get it to work with HMR.

So I had two goals:

1. I don't like Webpack, and I've come to prefer Parcel, at least for my simple projects where Parcel's promise of zero configuration works beautifully. If a project gests suitably complex that I need the Webpack overhead then sure, but otherwise I'd rather not have to think about it.
1. I wanted proper HMR, this means not some kind of auto-refresh, this is where the page updates without reloading so that you don't lose state. See the [comment from Dan Abramov on his video for HMR](https://vimeo.com/100010922#comment_12846462):

    > It's totally different. Livereload will trigger page refresh. It won't preserve the state of your components. Which makes editing complex UIs on the fly super irritating.

Parcel have made the lovely effort to support [ReasonML](https://parceljs.org/reasonML.html) and [React Hot Loader (RHL)](https://parceljs.org/hmr.html) out of the box. You need to include the `module.hot.accept` code snippet, but so does every RHL project from what I know.

## How

I made a couple of modifications to the template that comes from the `react-hooks` theme:

I switched the `bsconfig` to generate standard ES modules, as mentioned in the BuckleScript (now ReScript) [Config > package-specs docs](https://rescript-lang.org/docs/manual/latest/build-configuration#package-specs).

`bsconfig.json`:

```diff
   "package-specs": [{
-    "module": "commonjs",
+    "module": "es6-global",
     "in-source": true
   }],
```

Note that that probably isn't required, but it seems crazy to be generating CommonJS modules now that we've got ES modules as standard.

I deleted pretty much all the code from the src directory and left nothing but this in `src/Index.re`:

```reason
ReactDOMRe.renderToElementWithId(
  <h1>{React.string("Hello World")}</h1>,
  "root"
);
```

Then I added the code to include HMR that has been converted to Reason syntax thanks to this [gist](https://gist.github.com/rusty-key/f0412e79006c36a9c8ceda22e7495b6a), found via this [blog post](https://jeroenpelgrims.com/quick-and-dirty-react-reason-hmr-hot-module-reloading/):

```reason
[@bs.val] [@bs.scope ("module")]
// set isHotEnabled equal to module.hot
external isHotEnabled : bool = "hot";
// set hotAccept equal to module.hot.accept
[@bs.val] [@bs.scope ("module", "hot")] external hotAccept : unit => unit = "accept";

// enable HMR
if (isHotEnabled) {
  hotAccept();
}
```

Then I modified the `index.html` to refer directly to the `src/Index.re` file as in the [parcel docs](https://parceljs.org/reasonML.html#reasonml/bucklescript) and just include a basic `root` element:

```html
  <div id="root"></div>
  <script src="./src/Index.re"></script>
```

Then I updated the `package.json` to replace the 'start' script.

`package.json`:

```diff
   "scripts": {
     "build": "bsb -make-world",
+    "start": "parcel index.html",
-    "start": "bsb -make-world -w -ws _ ",
     "clean": "bsb -clean-world",
-    "moduleserver": "moduleserve ./ --port 8000",
     "test": "echo \"Error: no test specified\" && exit 1"
   },
```

Now running

```sh
yarn install
yarn start
```

Should get a page to appear at <http://localhost:1234/> which should reload changes to `src/Index.re` without a page refresh.

## Future

There is an upcoming [Parcel v2](https://v2.parceljs.org/) which includes the updated react-hot-loader with React [HMR Fast Refresh](https://v2.parceljs.org/recipes/react/#hmr-(fast-refresh)). I know this does work with React - but it doesn't work with ReasonML yet. My attempts at getting Parcel v2 with the `watcher.js` to update the Reason code failed in that all I could get was a basic page-refresh which ruins the whole purpose.
