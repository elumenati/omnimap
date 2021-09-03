
#include "OmniMapD3D10.h"
#include "OmniMapChannelD3D10.h"
#include "privatePrimitives.h"
#include "OmniMapShaderD3D10.h"
#include "OM_ErrorHandling.h"


OmniMapChannelD3D10::OmniMapChannelD3D10(ID3D10Device * _d3dDevice)
{
	d3dDevice = _d3dDevice;
  apOldRTVs =  NULL;
  pOldDS = NULL;

  channelShdrTexture = NULL;

  renderTargetTexture = NULL;
  renderTargetView = NULL;
  renderTargetSRV = NULL;

  depthStencilTarget = NULL;
  depthStencilView = NULL;

  useOmniMapMatrix = true;
  multiSampleQuality = 0.0;
  multiSampleCount = 1;
  SetUpPropertyAccess();
  hudVertexBuffer = NULL;
  hudWireframeBuffer = NULL;
  SetUpPropertyAccess();
}



OmniMapChannelD3D10::~OmniMapChannelD3D10()
{
  SAFE_RELEASE(renderTargetTexture);
  SAFE_RELEASE(renderTargetView);
  SAFE_RELEASE(channelShdrTexture);
  SAFE_RELEASE(renderTargetSRV);
  SAFE_RELEASE(hudVertexBuffer);
  SAFE_RELEASE(hudWireframeBuffer);
  SAFE_RELEASE(depthStencilTarget);
  SAFE_RELEASE(depthStencilView);
}




void OmniMapChannelD3D10::SetUpPropertyAccess()
{
	access.Register("MultiSampleQuality",&multiSampleQuality);
	OmniMapChannelBase::SetUpPropertyAccess();
}




DXGI_FORMAT OmniMapChannelD3D10::getDSFormat()
{
	// Determine whether we can and should support a multisampling on the HDR render target
  UINT SupportCaps = 0;

  d3dDevice->CheckFormatSupport( DXGI_FORMAT_D32_FLOAT, &SupportCaps );
  if( SupportCaps & D3D10_FORMAT_SUPPORT_DEPTH_STENCIL ) return DXGI_FORMAT_D32_FLOAT;

  d3dDevice->CheckFormatSupport( DXGI_FORMAT_D16_UNORM, &SupportCaps );
  if( SupportCaps & D3D10_FORMAT_SUPPORT_DEPTH_STENCIL ) return DXGI_FORMAT_D16_UNORM;

  d3dDevice->CheckFormatSupport( DXGI_FORMAT_D32_FLOAT_S8X24_UINT, &SupportCaps );
  if( SupportCaps & D3D10_FORMAT_SUPPORT_DEPTH_STENCIL ) return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

  return DXGI_FORMAT_UNKNOWN;
}




void OmniMapChannelD3D10::Initialize()
{
  ID3D10RenderTargetView* pOrigRT = NULL;
  D3D10_RENDER_TARGET_VIEW_DESC DescRTV;
  D3D10_TEXTURE2D_DESC tDesc;
  D3D10_TEXTURE2D_DESC dstex;
  D3D10_RENDER_TARGET_VIEW_DESC DescRT;
  DXGI_FORMAT dfmt = getDSFormat();
  D3D10_DEPTH_STENCIL_VIEW_DESC DescDS;
  D3D10_TEXTURE2D_DESC Desc;
  D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
  UINT pNumQualityLevels = 0;
  EH_DECLARE;

  EH_Log("Initialize");

  d3dDevice->OMGetRenderTargets( 1, &pOrigRT, NULL );

  EH_Ptr(pOrigRT);

  pOrigRT->GetDesc( &DescRTV );

#ifdef notdef
  if (multiSampleQuality > 0 || renderTarget == PBUFFER)
  {
#endif
    // Create the multisample render target
    UINT currentMultisampleQuality = (UINT)multiSampleQuality;
    pNumQualityLevels = 0;

    if (currentMultisampleQuality > 1) {
      EH_Test(d3dDevice->CheckMultisampleQualityLevels(DescRTV.Format, currentMultisampleQuality, &pNumQualityLevels));
      EH_Zero(pNumQualityLevels);
      pNumQualityLevels--;
    } else {
      currentMultisampleQuality = 1;
      pNumQualityLevels = 0;
    }

    EH_Log("Values Quality:%d, Count:%d", pNumQualityLevels, currentMultisampleQuality);

    tDesc.Width = xResolution;
    tDesc.Height = yResolution;
    tDesc.MipLevels = 1;
    tDesc.Format = DescRTV.Format;
    tDesc.SampleDesc.Quality = pNumQualityLevels;
    tDesc.SampleDesc.Count = currentMultisampleQuality;
    tDesc.Usage = D3D10_USAGE_DEFAULT;
    tDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    tDesc.CPUAccessFlags = 0;
    tDesc.MiscFlags = 0;
    tDesc.ArraySize = 1;
    EH_Test(d3dDevice->CreateTexture2D( &tDesc, NULL, &renderTargetTexture));

    // Create the render target view
    DescRT.Format = tDesc.Format;
    DescRT.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DMS;
    EH_Test(d3dDevice->CreateRenderTargetView( renderTargetTexture, &DescRT, &renderTargetView));

    //
    // Create depth stencil texture.
    //

    EH_Test(d3dDevice->CheckMultisampleQualityLevels(DescRTV.Format, (UINT)multiSampleQuality, &pNumQualityLevels));

    ZeroMemory( &dstex, sizeof( D3D10_TEXTURE2D_DESC ) );
    dstex.Width = xResolution;
    dstex.Height = yResolution;
    dstex.MipLevels = 1;
    dstex.Format = dfmt;
    tDesc.SampleDesc.Quality = pNumQualityLevels;
    tDesc.SampleDesc.Count = currentMultisampleQuality;
    dstex.Usage = D3D10_USAGE_DEFAULT;
    dstex.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = 0;
    dstex.ArraySize = 1;
    EH_Test(d3dDevice->CreateTexture2D( &dstex, NULL, &depthStencilTarget));

    // Create the depth stencil view
    ZeroMemory( &DescDS, sizeof( D3D10_DEPTH_STENCIL_VIEW_DESC ) );
    DescDS.Format = dfmt;
    DescDS.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;
    EH_Test(d3dDevice->CreateDepthStencilView( depthStencilTarget, &DescDS, &depthStencilView));
#ifdef notdef
  } else
  {
    // Create the render target texture
    ZeroMemory( &tDesc, sizeof( D3D10_TEXTURE2D_DESC ) );
    tDesc.ArraySize = 1;
    tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.Format =  DescRTV.Format;
    tDesc.Width = xResolution;
    tDesc.Height = yResolution;
    tDesc.MipLevels = 1;
    tDesc.SampleDesc.Count = 1;
    EH_Test(d3dDevice->CreateTexture2D( &tDesc, NULL, &renderTargetTexture ));

  }
#endif

  // Create the channel texture
  ZeroMemory( &Desc, sizeof( D3D10_TEXTURE2D_DESC ) );
  Desc.ArraySize = 1;
  Desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
  Desc.Usage = D3D10_USAGE_DEFAULT;
  Desc.Format = DescRTV.Format;
  Desc.Width = xResolution;
  Desc.Height = yResolution;
  Desc.MipLevels = 1;
  Desc.SampleDesc.Count = 1;
  EH_Test( d3dDevice->CreateTexture2D( &Desc, NULL, &channelShdrTexture ) );

  // Create the resource view
  ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
  SRVDesc.Format = tDesc.Format;
  SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  SRVDesc.Texture2D.MipLevels = tDesc.MipLevels;
  SRVDesc.Texture2D.MostDetailedMip = 0;
  EH_Test(d3dDevice->CreateShaderResourceView( channelShdrTexture, &SRVDesc, &renderTargetSRV ));


  EH_OnError() {
    // on error clear all created interfaces
    SAFE_RELEASE( renderTargetTexture );
    SAFE_RELEASE( renderTargetView );
    SAFE_RELEASE( depthStencilTarget );
    SAFE_RELEASE( depthStencilView );
    SAFE_RELEASE( renderTargetSRV );
    SAFE_RELEASE( channelShdrTexture );
  }


  // always clear temporary interfaces
  SAFE_RELEASE( pOrigRT );
}




void OmniMapChannelD3D10::GetProjectionMatrixRH(D3DXMATRIX *projMat)
{
	D3DXMatrixPerspectiveOffCenterRH(projMat, cam_info.ClipLeft, cam_info.ClipRight,
			cam_info.ClipBottom, cam_info.ClipTop, 
			cam_info.ClipNear, cam_info.ClipFar);
}



void OmniMapChannelD3D10::GetProjectionMatrix(float m[16])
{
  m;
}



void OmniMapChannelD3D10::GetProjectionMatrix(D3DXMATRIX *projMat)// assume projection space and identity
{
	
	D3DXMatrixPerspectiveOffCenterLH(projMat, cam_info.ClipLeft, cam_info.ClipRight,
		cam_info.ClipBottom, cam_info.ClipTop, cam_info.ClipNear, cam_info.ClipFar);
}




void OmniMapChannelD3D10::SetNearFar(float fnear, float ffar)
{
  float fovy = atan(cam_info.ClipTop/cam_info.ClipNear);
  float aspectRatio = (cam_info.ClipRight - cam_info.ClipLeft) /
    (cam_info.ClipTop - cam_info.ClipBottom);

  cam_info.ClipTop = tan(fovy) * fnear;		
  cam_info.ClipBottom = -cam_info.ClipTop;
  cam_info.ClipLeft   = aspectRatio*cam_info.ClipBottom;
  cam_info.ClipRight  = aspectRatio*cam_info.ClipTop;
  cam_info.ClipNear = fnear;
  cam_info.ClipFar = ffar;
}




void OmniMapChannelD3D10::ApplyProjectionMatrix()// assume projection space and identity
{

	D3DXMATRIX projectionMatrix;
    D3DXMatrixPerspectiveOffCenterLH(&projectionMatrix, cam_info.ClipLeft, cam_info.ClipRight,
		cam_info.ClipBottom, cam_info.ClipTop, cam_info.ClipNear, cam_info.ClipFar);
	// This needs to set a matrix in the shader
	//d3dDevice->SetTransform( D3DTS_PROJECTION, &projectionMatrix );
}



void OmniMapChannelD3D10::GetViewMatrix(D3DXMATRIX *vm)
{	
  D3DXMatrixIdentity( vm);
  AffMatrix am(cam_info.Matrix_WorldView);
  am.transpose();
  float *amvals = am.returnValues();
  vm->_11 = amvals[0];
  vm->_12 = amvals[1];
  vm->_13 = -amvals[2];

  vm->_21 = amvals[4];
  vm->_22 = amvals[5];
  vm->_23 = -amvals[6];

  vm->_31 = -amvals[8];
  vm->_32 = -amvals[9];
  vm->_33 = amvals[10];

  vm->_41 = amvals[12];
  vm->_42 = amvals[13];
  vm->_43 = -amvals[14];
  vm->_44 = 1.0;
}




void OmniMapChannelD3D10::GetViewMatrix2(D3DXMATRIX *vm)
{
  D3DXMatrixIdentity( vm);
  AffMatrix am(cam_info.Matrix_WorldView);
  float *amvals = am.returnValues();
  vm->_11 = amvals[0];
  vm->_12 = amvals[1];
  vm->_13 = amvals[2];
  vm->_14 = amvals[3];

  vm->_21 = amvals[4];
  vm->_22 = amvals[5];
  vm->_23 = amvals[6];
  vm->_24 = amvals[7];

  vm->_31 = amvals[8];
  vm->_32 = amvals[9];
  vm->_33 = amvals[10];
  vm->_34 = amvals[11];

  vm->_41 = amvals[12];
  vm->_42 = amvals[13];
  vm->_43 = amvals[14];
  vm->_44 = amvals[15];
}




void OmniMapChannelD3D10::ApplyViewMatrix() // assume world space
{
  D3DXMATRIX viewMatrix;
  D3DXMatrixIdentity( &viewMatrix);


  AffMatrix am(cam_info.Matrix_WorldView);
  am.transpose();
  AffMatrix ami;
  //am.setLookAtGL(OmniVec3(0.0f, 2.0f, -2.50f ), OmniVec3(0.0, 0.0, 0.0), OmniVec3(0.0,1.0,0.0));
  float *amvals = am.returnValues();
  float amarr[16];
  for (int i = 0; i < 16; i++)
    amarr[i] = amvals[i];

  viewMatrix._11 = amarr[0];
  viewMatrix._12 = amarr[1];
  viewMatrix._13 = -amarr[2];

  viewMatrix._21 = amarr[4];
  viewMatrix._22 = amarr[5];
  viewMatrix._23 = -amarr[6];

  viewMatrix._31 = -amarr[8];
  viewMatrix._32 = -amarr[9];
  viewMatrix._33 = amarr[10];

  viewMatrix._41 = amarr[12];
  viewMatrix._42 = amarr[13];
  viewMatrix._43 = -amarr[14];
  viewMatrix._44 = 1.0;

  //Needs to do something with a shader variable.
  //d3dDevice->MultiplyTransform( D3DTS_PROJECTION, &viewMatrix );
}




void OmniMapChannelD3D10::GetProjTextMatrix(float m[16])
{
  D3DXQUATERNION tmpQuat;
  D3DXMATRIX textureMat;
  D3DXMATRIX tmpMat;
  D3DXMatrixIdentity(&textureMat);
  const D3DXVECTOR3 zeroVec(0.0,0.0,0.0);
  const D3DXVECTOR3 transVec(.5,.5,.5);

  D3DXMatrixAffineTransformation(&textureMat, .5, &zeroVec, D3DXQuaternionIdentity(&tmpQuat),
    &transVec);	

  D3DXMATRIX channelProj;
  GetProjectionMatrixRH(&channelProj);

  D3DXMatrixMultiply(&textureMat, &channelProj, &textureMat);
  D3DXMATRIX viewMatrix;
  GetViewMatrix2(&viewMatrix);
  D3DXMatrixMultiply(&textureMat, &viewMatrix, &textureMat);
  for (int i = 0; i < 16; i++)
  {
    m[i] = textureMat[i];
  }
}




void OmniMapChannelD3D10::GetProjTextMatrix(D3DXMATRIX *ptMat)
{
  D3DXQUATERNION tmpQuat;
  D3DXMATRIX textureMat;
  D3DXMATRIX tmpMat;
  D3DXMatrixIdentity(&textureMat);
  const D3DXVECTOR3 zeroVec(0.0,0.0,0.0);
  const D3DXVECTOR3 transVec(.5,.5,.5);

  D3DXMatrixAffineTransformation(&textureMat, .5, &zeroVec, D3DXQuaternionIdentity(&tmpQuat),
    &transVec);	

  D3DXMATRIX channelProj;
  GetProjectionMatrixRH(&channelProj);

  D3DXMatrixMultiply(&textureMat, &channelProj, &textureMat);
  D3DXMATRIX viewMatrix;
  GetViewMatrix2(&viewMatrix);
  D3DXMatrixMultiply(ptMat, &viewMatrix, &textureMat);
}




void OmniMapChannelD3D10::beginRenderToChannel()
{
  float ClearColor[4] = { 0.0, 0.0, 0.0, 0.0 };
  D3D10_VIEWPORT vp;
  UINT one = 1;

  // Save the old RT and DS buffer views
  d3dDevice->OMGetRenderTargets( 1, &apOldRTVs, &pOldDS );

  d3dDevice->ClearRenderTargetView( renderTargetView, ClearColor);
  d3dDevice->ClearDepthStencilView( depthStencilView, D3D10_CLEAR_DEPTH, 1.0, 0);

  d3dDevice->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
  d3dDevice->RSGetViewports(&one, &vpSave);
  vp.Height = xResolution;
  vp.Width = yResolution;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  vp.MaxDepth = vpSave.MaxDepth;
  vp.MinDepth = vpSave.MinDepth;
  d3dDevice->RSSetViewports(1, &vp);

#ifdef NOTYET
  if (multiSampleQuality > 0 || renderTarget == PBUFFER)
  {

  } else if (renderTarget == BACK_BUFFER)
  {

  } else
  {

  }

  if (FAILED(hr))
    LogSystem()->ReportError("Cannot set render surface.");
#endif	      
  if (useOmniMapMatrix)
    PushSetupMatricies();
}



void OmniMapChannelD3D10::endRenderToChannel()
{
  D3D10_TEXTURE2D_DESC Desc;

  // Restore old RT and DS buffer views
  d3dDevice->OMSetRenderTargets( 1, &apOldRTVs, pOldDS );
  d3dDevice->RSSetViewports(1, &vpSave);

  channelShdrTexture->GetDesc( &Desc );
  d3dDevice->ResolveSubresource( channelShdrTexture, D3D10CalcSubresource( 0, 0, 1 ), 
    renderTargetTexture, D3D10CalcSubresource( 0, 0, 1 ), Desc.Format );

  SAFE_RELEASE(apOldRTVs);
  SAFE_RELEASE(pOldDS);
}




void OmniMapChannelD3D10::PushSetupMatricies()
{
}



void OmniMapChannelD3D10::PopSetupMatricies()
{
}




void OmniMapChannelD3D10::BindTexture(int index)
{
#ifdef NOTYET
  OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
  OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ) );
  OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP) );
  OMNIMAP_V( d3dDevice->SetSamplerState( index, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP) );
#endif
  omShader->setTexture(index, renderTargetSRV);
  //OmniMapD3D10::textureTable[index] = renderTargetSRV;
}






void OmniMapChannelD3D10::UnbindTexture(int index)
{
	omShader->setTexture(index, NULL);
}
// Create a screen quad for all render to texture operations
    


struct TexVertex10
{
  D3DXVECTOR3 pos;
  D3DXVECTOR2 tex;
};



void OmniMapChannelD3D10::CreateHUDSurface()
{
  TexVertex10 svQuad[4];
  EH_DECLARE;

  svQuad[0].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.bl.x, HeadsUpDisplay_Quad.bl.y,  HeadsUpDisplay_Quad.bl.z);
  svQuad[0].tex = D3DXVECTOR2(0.0f, 1.0f);
  svQuad[1].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y, HeadsUpDisplay_Quad.tl.z);
  svQuad[1].tex = D3DXVECTOR2(0.0f, 0.0f);
  svQuad[2].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.br.x, HeadsUpDisplay_Quad.br.y, HeadsUpDisplay_Quad.br.z);
  svQuad[2].tex = D3DXVECTOR2(1.0f, 1.0f);
  svQuad[3].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.tr.x, HeadsUpDisplay_Quad.tr.y, HeadsUpDisplay_Quad.tr.z);
  svQuad[3].tex = D3DXVECTOR2(1.0f, 0.0f);

  D3D10_BUFFER_DESC vbdesc =
  {
    4*sizeof(TexVertex10),
    D3D10_USAGE_IMMUTABLE,
    D3D10_BIND_VERTEX_BUFFER,
    0,
    0
  };

  D3D10_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = svQuad;
  InitData.SysMemPitch = 0;
  InitData.SysMemSlicePitch = 0;
  EH_Test(d3dDevice->CreateBuffer( &vbdesc, &InitData, &hudVertexBuffer ));

  EH_OnError() {}


/*
	ColVertex10 cvQuad[4];
    cvQuad[0].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.bl.x, HeadsUpDisplay_Quad.bl.y,  HeadsUpDisplay_Quad.bl.z);
    cvQuad[0].col = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
    cvQuad[1].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.tl.x,HeadsUpDisplay_Quad.tl.y, HeadsUpDisplay_Quad.tl.z);
    cvQuad[1].col = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
	cvQuad[2].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.tr.x, HeadsUpDisplay_Quad.tr.y, HeadsUpDisplay_Quad.tr.z);
    cvQuad[2].col = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );
    cvQuad[3].pos = D3DXVECTOR3(HeadsUpDisplay_Quad.br.x, HeadsUpDisplay_Quad.br.y, HeadsUpDisplay_Quad.br.z);
    cvQuad[3].col = D3DCOLOR_COLORVALUE( 0.0, 0.0, 0.0, 1.0 );

	D3D10_BUFFER_DESC cvbdesc =
    {
        4*sizeof(ColVertex10),
        D3D10_USAGE_DEFAULT,
        D3D10_BIND_VERTEX_BUFFER,
        0,
        0
    };

	D3D10_SUBRESOURCE_DATA cvInitData;
    cvInitData.pSysMem = cvQuad;
    cvInitData.SysMemPitch = sizeof(cvQuad);
    cvInitData.SysMemSlicePitch = sizeof(cvQuad);
    hr = d3dDevice->CreateBuffer( &cvbdesc, &cvInitData, &hudWireframeBuffer );
*/
}



void OmniMapChannelD3D10::setOMShader(OmniMapShaderD3D10 *shader)
{
	omShader = shader;
}



void OmniMapChannelD3D10::DrawHeadsUpDisplay()
{
  if (hudVertexBuffer == NULL) 
  {
    CreateHUDSurface();
  }

  UINT uStrides = sizeof(TexVertex10);
  UINT uOffsets = 0;
  ID3D10Buffer *pBuffers[1] = { hudVertexBuffer };
  d3dDevice->IASetVertexBuffers( 0, 1, pBuffers, &uStrides, &uOffsets );
  d3dDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
  d3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

  // Draw
  D3D10_TECHNIQUE_DESC techDesc;

  ID3D10EffectTechnique* pTech = omShader->EnableHUDShader(renderTargetSRV);

  pTech->GetDesc( &techDesc );
  for( UINT p = 0; p < techDesc.Passes; ++p )
  {
    pTech->GetPassByIndex( p )->Apply(0);
    d3dDevice->Draw( 4, 0 );
  }
}


