from django.urls import path
from .views import activity_log, dashboard_data, generate_report,barrier_open

urlpatterns = [
    path('dashboard_data/', dashboard_data, name='dashboard_data'),
    path('generate_report/', generate_report, name='generate_report'),
    path('activity_log/', activity_log, name='activity_log'),
    path('barrier_open/', barrier_open, name='barrier_open'),
]