from django.urls import path
from .views import login_view,logout_view

urlpatterns = [
    path('login/', login_view, name='login'),
    path('signout/', logout_view, name='signout'),
]
