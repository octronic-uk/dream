App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {

        $scope.project = ProjectService.getProject();
        $scope.newResource = ProjectService.createAnimationResource();

        UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceAddButtonClicked = function() {
            ProjectService.pushResource($scope.newResource);
            $scope.newResource = ProjectService.createAnimationResource();
        };

        $scope.onNewResourceClearButtonClicked = function() {
            $scope.newResource = ProjectService.createAnimationResource();
        };
    }
]);