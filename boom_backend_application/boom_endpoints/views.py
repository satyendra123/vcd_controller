import datetime

from django.shortcuts import render
from rest_framework.decorators import api_view
from rest_framework.response import Response
from .models import ActivityLog, BoomSig
from .serializers import ActivityLogSerializer


@api_view(['GET'])
def open_boom(request):
    boom_sig_instance = BoomSig.objects.get(id=1)
    boom_sig_instance.entryboom = 'Y'
    boom_sig_instance.save()

    ActivityLog.objects.create()

    return Response(
        {
            "status": "OK"
        }
    )

@api_view(['GET'])
def get_activity_logs(request):
    activity_logs = ActivityLog.objects.filter()
    serializer_data = ActivityLogSerializer(activity_logs, many=True)

    return Response(
        {
            "data": serializer_data.data or {},
            "status": "OK"
        }
    )