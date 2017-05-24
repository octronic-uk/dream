App.service('CookieService',
    ['$cookieStore',
    function($cookieStore){

        var COOKIE_EXEC_PATH = "dream-bin";
        var COOKIE_THEME     = "theme";
        var DEFAULT_THEME    = "simplex";
        var DEFAULT_PATH     = "/Users/Ashley/software/build-Dream-Apple_GCC-Debug/src/Dream";

        this.setTheme = function(theme) {
            $cookieStore.put(COOKIE_THEME,theme);
        };

        this.getTheme = function() {
            return $cookieStore.get(COOKIE_THEME) || DEFAULT_THEME;
        };

        this.setDreamExecPath = function(path) {
            $cookieStore.put(COOKIE_EXEC_PATH,path);
        };

        this.getDreamExecPath = function() {
            return $cookieStore.get(COOKIE_EXEC_PATH) || DEFAULT_PATH;
        };

        return this;
    }
]);