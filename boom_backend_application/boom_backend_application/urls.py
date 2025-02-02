from django.contrib import admin
from django.urls import path, include


urlpatterns = [
    path('api/auth/', include('authentication.urls')),
    path("admin/", admin.site.urls),
    path('api/boomendpoints/', include('boom_endpoints.urls')),
]