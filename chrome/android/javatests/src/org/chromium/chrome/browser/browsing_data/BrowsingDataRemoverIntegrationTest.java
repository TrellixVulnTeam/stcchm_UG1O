// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.browsing_data;

import android.content.Intent;
import android.os.AsyncTask;
import android.test.suitebuilder.annotation.MediumTest;

import org.chromium.base.ThreadUtils;
import org.chromium.base.test.util.RetryOnFailure;
import org.chromium.chrome.browser.ChromeActivity;
import org.chromium.chrome.browser.ShortcutHelper;
import org.chromium.chrome.browser.preferences.PrefServiceBridge;
import org.chromium.chrome.browser.preferences.PrefServiceBridge.OnClearBrowsingDataListener;
import org.chromium.chrome.browser.webapps.WebappDataStorage;
import org.chromium.chrome.browser.webapps.WebappRegistry;
import org.chromium.chrome.test.ChromeActivityTestCaseBase;
import org.chromium.content.browser.test.util.Criteria;
import org.chromium.content.browser.test.util.CriteriaHelper;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Integration tests for the native BrowsingDataRemover.
 *
 * BrowsingDataRemover is used to delete data from various data storage backends. However, for
 * those backends that live in the Java code, it is not possible to test whether deletions were
 * successful in its own unit tests. This test can do so.
 */
public class BrowsingDataRemoverIntegrationTest extends ChromeActivityTestCaseBase<ChromeActivity> {
    private boolean mCallbackCalled;

    private class CallbackCriteria extends Criteria {
        public CallbackCriteria() {
            mCallbackCalled = false;
        }

        @Override
        public boolean isSatisfied() {
            if (mCallbackCalled) {
                mCallbackCalled = false;
                return true;
            }
            return false;
        }
    }

    public BrowsingDataRemoverIntegrationTest() {
        super(ChromeActivity.class);
    }

    @Override
    public void startMainActivity() throws InterruptedException {
        startMainActivityOnBlankPage();
    }

    private void registerWebapp(final String webappId, final String webappUrl) throws Exception {
        AsyncTask<Void, Void, Intent> shortcutIntentTask = new AsyncTask<Void, Void, Intent>() {
            @Override
            protected Intent doInBackground(Void... nothing) {
                return ShortcutHelper.createWebappShortcutIntentForTesting(webappId, webappUrl);
            }
        };
        final Intent shortcutIntent = shortcutIntentTask.execute().get();

        WebappRegistry.registerWebapp(
                webappId, new WebappRegistry.FetchWebappDataStorageCallback() {
                    @Override
                    public void onWebappDataStorageRetrieved(WebappDataStorage storage) {
                        storage.updateFromShortcutIntent(shortcutIntent);
                        mCallbackCalled = true;
                    }
                });

        CriteriaHelper.pollUiThread(new CallbackCriteria());
    }

    /**
     * Tests that web apps are unregistered after clearing with the "cookies and site data" option.
     * TODO(msramek): Expose more granular datatypes to the Java code, so we can directly test
     * BrowsingDataRemover::RemoveDataMask::REMOVE_WEBAPP_DATA instead of BrowsingDataType.COOKIES.
     */
    @MediumTest
    @RetryOnFailure
    public void testUnregisteringWebapps() throws Exception {
        // Register three web apps.
        final HashMap<String, String> apps = new HashMap<String, String>();
        apps.put("webapp1", "https://www.google.com/index.html");
        apps.put("webapp2", "https://www.chrome.com/foo/bar");
        apps.put("webapp3", "http://example.com/");

        for (final Map.Entry<String, String> app : apps.entrySet()) {
            registerWebapp(app.getKey(), app.getValue());
        }

        // Wait for the registration to finish.
        WebappRegistry.getRegisteredWebappIds(new WebappRegistry.FetchCallback() {
            @Override
            public void onWebappIdsRetrieved(Set<String> ids) {
                assertEquals(apps.keySet(), ids);
                mCallbackCalled = true;
            }
        });
        CriteriaHelper.pollUiThread(new CallbackCriteria());

        // Clear cookies and site data excluding the registrable domain "google.com".
        ThreadUtils.runOnUiThreadBlocking(new Runnable() {
            @Override
            public void run() {
                PrefServiceBridge.getInstance().clearBrowsingDataExcludingDomains(
                        new OnClearBrowsingDataListener() {
                            @Override
                            public void onBrowsingDataCleared() {
                                mCallbackCalled = true;
                            }
                        },
                        new int[]{ BrowsingDataType.COOKIES },
                        TimePeriod.ALL_TIME,
                        new String[]{ "google.com" },
                        new int[] { 1 },
                        new String[0],
                        new int[0]);
            }
        });
        CriteriaHelper.pollUiThread(new CallbackCriteria());

        // The last two webapps should have been unregistered.
        WebappRegistry.getRegisteredWebappIds(new WebappRegistry.FetchCallback() {
            @Override
            public void onWebappIdsRetrieved(Set<String> ids) {
                assertEquals(new HashSet<String>(Arrays.asList("webapp1")), ids);
                mCallbackCalled = true;
            }
        });
        CriteriaHelper.pollUiThread(new CallbackCriteria());

        // Clear cookies and site data with no url filter.
        ThreadUtils.runOnUiThreadBlocking(new Runnable() {
            @Override
            public void run() {
                PrefServiceBridge.getInstance().clearBrowsingData(
                        new OnClearBrowsingDataListener() {
                            @Override
                            public void onBrowsingDataCleared() {
                                mCallbackCalled = true;
                            }
                        },
                        new int[]{ BrowsingDataType.COOKIES },
                        TimePeriod.ALL_TIME);
            }
        });
        CriteriaHelper.pollUiThread(new CallbackCriteria());

        // All webapps should have been unregistered.
        WebappRegistry.getRegisteredWebappIds(new WebappRegistry.FetchCallback() {
            @Override
            public void onWebappIdsRetrieved(Set<String> ids) {
                assertTrue(ids.isEmpty());
                mCallbackCalled = true;
            }
        });
        CriteriaHelper.pollUiThread(new CallbackCriteria());
    }
}
