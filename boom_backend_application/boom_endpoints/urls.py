from django.urls import path
from . import views


urlpatterns = [
    path("open/boom", views.open_boom, name="open_boom"),
    path("get/activitylogs", views.get_activity_logs, name="get_activity_logs"),
]
