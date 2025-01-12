var UnitTest = {};
(function()
{
    var oldLoadResourcePromise = Runtime.loadResourcePromise;
    Runtime.loadResourcePromise = function(url)
    {
        if (url.startsWith("/"))
            return oldLoadResourcePromise(url);

        if (!url.startsWith("http://"))
            return oldLoadResourcePromise("/inspector-debug/" + url);

        var parsedURL = new URL(url, location.href);
        var parsedLocation = new URL(location.href);

        // hosted devtools is confused.
        parsedURL.pathname = parsedURL.pathname.replace('/inspector-unit/', '/inspector-debug/');
        return oldLoadResourcePromise(parsedURL.toString());
    }

    if (window.testRunner) {
        testRunner.dumpAsText();
        testRunner.waitUntilDone();
    }

    var results = [];
    UnitTest.completeTest = function()
    {
        if (!window.testRunner) {
            console.log("Test Done");
            return;
        }

        document.documentElement.childNodes.forEach(x => x.remove());
        var outputElement = document.createElement("div");
        // Support for svg - add to document, not body, check for style.
        if (outputElement.style) {
            outputElement.style.whiteSpace = "pre";
            outputElement.style.height = "10px";
            outputElement.style.overflow = "hidden";
        }
        document.documentElement.appendChild(outputElement);
        for (var i = 0; i < results.length; i++) {
            outputElement.appendChild(document.createTextNode(results[i]));
            outputElement.appendChild(document.createElement("br"));
        }
        results = [];
        testRunner.notifyDone();
    }

    UnitTest.addResult = function(text)
    {
        if (window.testRunner)
            results.push(String(text));
        else
            console.log(text);
    }

    UnitTest.runTests = function(tests)
    {
        nextTest();

        function nextTest()
        {
            var test = tests.shift();
            if (!test) {
                UnitTest.completeTest();
                return;
            }
            UnitTest.addResult("\ntest: " + test.name);
            var testPromise = test();
            if (!(testPromise instanceof Promise))
                testPromise = Promise.resolve();
            testPromise.then(nextTest);
        }
    }

    function completeTestOnError(message, source, lineno, colno, error)
    {
        UnitTest.addResult("TEST ENDED IN ERROR: " + error.stack);
        UnitTest.completeTest();
    }
    window.onerror = completeTestOnError;

    Runtime.startApplication("/inspector-unit/inspector-unit-test").then(runTest);

    function runTest()
    {
        var description = document.body.textContent.trim();
        if (description)
            UnitTest.addResult(description);

        WebInspector.settings = new WebInspector.Settings(new WebInspector.SettingsStorage(
            {},
            InspectorFrontendHost.setPreference,
            InspectorFrontendHost.removePreference,
            InspectorFrontendHost.clearPreferences));

        WebInspector.viewManager = new WebInspector.ViewManager();
        WebInspector.initializeUIUtils(document, WebInspector.settings.createSetting("uiTheme", "default"));

        WebInspector.zoomManager = new WebInspector.ZoomManager(window, InspectorFrontendHost);
        WebInspector.inspectorView = WebInspector.InspectorView.instance();
        WebInspector.ContextMenu.initialize();
        WebInspector.ContextMenu.installHandler(document);
        WebInspector.Tooltip.installHandler(document);

        var rootView = new WebInspector.RootView();
        WebInspector.inspectorView.show(rootView.element);
        WebInspector.inspectorView.showInitialPanel();
        rootView.attachToDocument(document);

        test();
    }
})();
