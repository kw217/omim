//
//  MWMAlertViewController.m
//  Maps
//
//  Created by v.mikhaylenko on 05.03.15.
//  Copyright (c) 2015 MapsWithMe. All rights reserved.
//

#import "MWMAlertViewController.h"
#import "MWMDownloadTransitMapAlert.h"
#import "MWMAlertEntity.h"
#import "MWMAlertViewControllerDelegate.h"

static NSString * const kAlertControllerNibIdentifier = @"MWMAlertViewController";

@interface MWMAlertViewController () <UIGestureRecognizerDelegate>
@property (nonatomic, weak, readwrite) UIViewController *ownerViewController;
@property (nonatomic, weak) IBOutlet UITapGestureRecognizer *tap;
@end

@implementation MWMAlertViewController

- (instancetype)initWithViewController:(UIViewController *)viewController {
  self = [super initWithNibName:kAlertControllerNibIdentifier bundle:nil];
  if (self) {
    self.ownerViewController = viewController;
  }
  return self;
}

- (void)viewDidLoad {
  [super viewDidLoad];
  self.view.frame = UIApplication.sharedApplication.delegate.window.bounds;
  
  //Need only for iOS 5
  if ([[[UIDevice currentDevice] systemVersion] integerValue] < 6) {
    self.tap.delegate = self;
  }
}

#pragma mark - Actions

- (void)presentAlertWithType:(MWMAlertType)type {
  MWMAlert *alert = [MWMAlert alertWithType:type];
  alert.alertController = self;
  switch (type) {
    case MWMAlertTypeDownloadTransitMap: {
      MWMAlertEntity *entity = [[MWMAlertEntity alloc] init];
      entity.country = self.delegate.countryName;
      entity.size = self.delegate.size;
      [alert configureWithEntity:entity];
      break;
    }
    case MWMAlertTypeDownloadAllMaps:
    case MWMAlertTypeRouteNotFound:
      
      break;
  }
  [self.ownerViewController addChildViewController:self];
  self.view.center = self.ownerViewController.view.center;
  [self.ownerViewController.view addSubview:self.view];
  [self.view addSubview:alert];
  alert.center = self.view.center;
}

- (void)close {
  self.ownerViewController.view.userInteractionEnabled = YES;
  [self.view removeFromSuperview];
  [self removeFromParentViewController];
}

- (IBAction)backgroundTap:(UITapGestureRecognizer *)sender {
  return;
}

- (IBAction)backgroundSwipe:(UISwipeGestureRecognizer *)sender {
  return;
}

- (IBAction)backgroundPinch:(id)sender {
  return;
}

- (IBAction)backgroundPan:(id)sender {
  return;
}

#pragma mark - Gesture Recognizer Delegate

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch {
  if ([touch.view isKindOfClass:[UIControl class]]) {
    [(UIButton *)touch.view sendActionsForControlEvents:UIControlEventTouchUpInside];
  }
  return YES;
}

@end