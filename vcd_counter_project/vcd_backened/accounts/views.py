from django.contrib.auth import authenticate
from django.http import JsonResponse
from .utils.jwt_utils import generate_jwt
from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import AllowAny
import json
from django.contrib.auth.models import User
from rest_framework.permissions import IsAuthenticated
from rest_framework_simplejwt.authentication import JWTAuthentication
from rest_framework_simplejwt.tokens import RefreshToken
from rest_framework.exceptions import AuthenticationFailed

@api_view(['POST'])
@permission_classes([AllowAny])
def login_view(request):
        data = json.loads(request.body)
        username = data.get('username')
        password = data.get('password')
        print(username)
        print(password)
        try:
            user = User.objects.get(username=username)
        except User.DoesNotExist:
            return JsonResponse({'error': 'Invalid credentials'}, status=400)

        if password == user.password:
            token = generate_jwt(user)
            return JsonResponse({'status': 'Success', 'token': token}, status=200)
        else:
            return JsonResponse({'error': 'Invalid credentials'}, status=400)

@api_view(["POST"])
@permission_classes([IsAuthenticated])
def logout_view(request):
    try:
        auth_header = request.headers.get("Authorization", "")
        if auth_header.startswith("Bearer "):
            token = auth_header.split(" ")[1]
            jwt_auth = JWTAuthentication()
            validated_token = jwt_auth.get_validated_token(token)
            refresh_token = RefreshToken(token)
            refresh_token.blacklist()  # Blacklist token if using JWT
            
        else:
            return JsonResponse({"error": "Invalid token format"}, status=400)
            
    except AuthenticationFailed as e:
        return JsonResponse({"error": str(e)}, status=401)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=400)
    
    return JsonResponse({"message": "Signed out successfully"}, status=200)
